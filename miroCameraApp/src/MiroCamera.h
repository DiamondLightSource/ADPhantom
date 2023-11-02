// Standard includes
#include <vector>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>
#include <sstream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>
#include <locale>
#include <map>
#include <algorithm>
#include <ctime>

// EPICS includes
#include <epicsThread.h>
#include <epicsEvent.h>
#include <epicsString.h>
#include <iocsh.h>
#include <drvSup.h>
#include <epicsExport.h>

// areaDetector includes
#include <ADDriver.h>

// Asyn driver includes
#include "asynOctetSyncIO.h"

// Phantom camera PH16 protocol data definitions
#include <ph16UnitStructure.h>

// Protocol buffer size is 64K
#define MIRO_MAX_STRING 8192
// Asyn timeout
#define MIRO_TIMEOUT 1.000
#define MIRO_EXTENDED_TIMEOUT 200.0 // Format time out
// MIRO Update rate
#define MIRO_UPDATE_RATE 0.1
// MIRO Response OK/ERROR
#define MIRO_OK_STRING    "Ok!"
#define MIRO_ERROR_STRING "ERR:"

// MIRO Run Modes
#define MIRO_RUN_FAT  0
#define MIRO_RUN_SFAT 1
#define MIRO_RUN_FRR  2
#define MIRO_RUN_FE   3

// MIRO Cine States
#define MIRO_CINE_STATE_INV 1
#define MIRO_CINE_STATE_STR 2
#define MIRO_CINE_STATE_WTR 4
#define MIRO_CINE_STATE_TRG 8
#define MIRO_CINE_STATE_RDY 16
#define MIRO_CINE_STATE_DEF 32
#define MIRO_CINE_STATE_ABL 64
#define MIRO_CINE_STATE_PRE 128
#define MIRO_CINE_STATE_ACT 256
#define MIRO_CINE_STATE_REU 512

// MIRO Data Types
#define MIRO_TYPE_FLOAT   "float"
#define MIRO_TYPE_INTEGER "integer"
#define MIRO_TYPE_STRING  "string"
#define MIRO_TYPE_HEX     "hex"
#define MIRO_TYPE_RES     "resolution"
#define MIRO_TYPE_FLAGS   "flaglist"

// Number of Cines
#define MIRO_NUMBER_OF_CINES  17

// Number of flash files to display at one time
#define MIRO_NUMBER_OF_FLASH_FILES 8

// MIRO Command Strings
#define MIRO_CMD_GET_VALUE    "get"
#define MIRO_CMD_SET_VALUE    "set"
#define MIRO_CMD_ABORT        "rec 0"
#define MIRO_CMD_START_REC    "rec"
#define MIRO_CMD_DELETE       "del"
#define MIRO_CMD_RELEASE      "rel"
#define MIRO_CMD_TRIG         "trig"
#define MIRO_CMD_GET_CSTATES  "cstats"
#define MIRO_CMD_START_DATA   "startdata"
#define MIRO_CMD_ATTACH       "attach"
#define MIRO_CMD_GET_IMG      "img"
#define MIRO_CMD_GET_XIMG     "ximg"
#define MIRO_CMD_GET_TSTAMP   "time"
#define MIRO_CMD_PARTITION    "partition"
#define MIRO_CMD_WHITEBAL     "wbal"
#define MIRO_CMD_WUPDATE      "wupdate"
#define MIRO_CMD_BLACKREF     "bref"
#define MIRO_CMD_BUPDATE      "bupdate"
#define MIRO_CMD_FERASE       "ferase"
#define MIRO_CMD_FSAVE        "fsave"
#define MIRO_CMD_CFSAVE       "cfsave"
#define MIRO_CMD_GET_LOG      "tail"
#define MIRO_CMD_VPLAY        "vplay"
#define MIRO_CMD_CHECKCLEAN   "clean"
#define MIRO_CMD_ENABNOTIFY   "notify"
#define MIRO_CMD_USERSAVE     "usave"
#define MIRO_CMD_USERLOAD     "uload"
#define MIRO_CMD_USERERASE    "uerase"
#define MIRO_CMD_USERLIST     "uls"
#define MIRO_CMD_DEBUG        "console"
#define MIRO_CMD_SET_FSTOP    "fstop"
#define MIRO_CMD_FOCUS        "focus"
#define MIRO_CMD_CALIB        "calib"
#define MIRO_CMD_SYSMON       "sysmon"
#define MIRO_CMD_TESTIMG      "testimg"
#define MIRO_CMD_SETRTC       "setrtc"
#define MIRO_CMD_CFLS         "cfls"
#define MIRO_CMD_CFRM         "cfrm"
#define MIRO_CMD_CFFORMAT     "cfformat"
#define MIRO_CMD_CFREAD       "cfread"
#define MIRO_CMD_PRESET       "preset"
#define MIRO_CMD_MMSET        "mmset"

// Pre-defined EPICS Parameter Names
#define MIROConnectString                   "MIRO_CONNECT"
#define MIROConnectedString                 "MIRO_CONNECTED"

#define MIRO_AcquireStateString             "MIRO_ACQUIRE_STATE"

#define MIRO_SoftwareTriggerString          "MIRO_SOFTWARE_TRIGGER"
#define MIRO_PerformCSRString               "MIRO_PERFORM_CSR"
#define MIRO_CSRCountString                 "MIRO_CSR_COUNT"

#define MIRO_SettingsSlotString             "MIRO_SETTINGS_SLOT"
#define MIRO_SettingsSaveString             "MIRO_SETTINGS_SAVE"
#define MIRO_SettingsLoadString             "MIRO_SETTINGS_LOAD"

#define MIRO_AutoSaveString                 "MIRO_AUTO_SAVE"
#define MIRO_AutoRestartString              "MIRO_AUTO_RESTART"
#define MIRO_AutoCSRString                  "MIRO_AUTO_CSR"

#define MIRO_PostTrigCountString            "MIRO_POST_TRIG_COUNT"
#define MIRO_TotalFrameCountString          "MIRO_TOTAL_FRAME_COUNT"
#define MIRO_MaxFrameCountString            "MIRO_MAX_FRAME_COUNT"

#define MIRO_CineNameString                 "MIRO_CINE_NAME"
#define MIRO_SelectedCineString             "MIRO_CINE_SELECTED"
#define MIRO_CineWidthString                "MIRO_CINE_WIDTH"
#define MIRO_CineHeightString               "MIRO_CINE_HEIGHT"
#define MIRO_CineFrameCountString           "MIRO_CINE_FR_COUNT"
#define MIRO_CineFirstFrameString           "MIRO_CINE_FIRST_FR"
#define MIRO_CineLastFrameString            "MIRO_CINE_LAST_FR"
#define MIRO_CineRecordStartString          "MIRO_CINE_REC_START"
#define MIRO_CineRecordEndString            "MIRO_CINE_REC_END"
#define MIRO_CineRecordString               "MIRO_CINE_RECORD"
#define MIRO_CineRecordCountString          "MIRO_CINE_REC_COUNT"
#define MIRO_CineSaveCFString               "MIRO_CINE_SAVE_CF"       // Save selected cine to flash

#define MIRO_SetPartitionString             "MIRO_SET_PARTITION"
#define MIRO_GetCineCountString             "MIRO_GET_CINE_COUNT"

#define MIRO_CFStateString                  "MIRO_CF_STATE"
#define MIRO_CFActionString                 "MIRO_CF_ACTION"
#define MIRO_CFSizeString                   "MIRO_CF_SIZE"
#define MIRO_CFUsedString                   "MIRO_CF_USED"
#define MIRO_CFProgressString               "MIRO_CF_PROGRESS"
#define MIRO_CFErrorString                  "MIRO_CF_ERROR"
#define MIRO_CFNumFilesString               "MIRO_CF_NUM_FILES"
#define MIRO_CFMaxPagesString               "MIRO_CF_MAX_PAGES"
#define MIRO_CFCurPageString                "MIRO_CF_CUR_PAGE"
#define MIRO_CFFormatString                 "MIRO_CF_FORMAT"

#define MIRO_CFFileNameString               "MIRO_CF_FILENAME"
#define MIRO_CFSIndexString                 "MIRO_CFS_INDEX"
#define MIRO_CFSWidthString                 "MIRO_CFS_WIDTH"
#define MIRO_CFSHeightString                "MIRO_CFS_HEIGHT"
#define MIRO_CFSFrameCountString            "MIRO_CFS_FR_COUNT"
#define MIRO_CFSFirstFrameString            "MIRO_CFS_FIRST_FR"
#define MIRO_CFSLastFrameString             "MIRO_CFS_LAST_FR"
#define MIRO_CFSRecordStartString           "MIRO_CFS_REC_START"
#define MIRO_CFSRecordEndString             "MIRO_CFS_REC_END"
#define MIRO_CFSRecordString                "MIRO_CFS_RECORD"
#define MIRO_CFSRecordCountString           "MIRO_CFS_REC_COUNT"
#define MIRO_CFSFileDeleteString            "MIRO_CFS_DELETE"         // Delete the (C)ompact (F)lash (S)elected file

#define MIRO_InfoSensorTempString           "MIRO_SENSOR_TEMP"
#define MIRO_InfoThermoPowerString          "MIRO_THERMO_POWER"
#define MIRO_InfoCameraTempString           "MIRO_CAMERA_TEMP"
#define MIRO_InfoFanPowerString             "MIRO_FAN_POWER"
#define MIRO_EDRString                      "MIRO_EDR"
#define MIRO_CamExtSyncString               "MIRO_EXT_SYNC"
#define MIRO_CamFrameDelayString            "MIRO_FRAME_DELAY"
#define MIRO_CamTriggerEdgeString           "MIRO_TRIGGER_EDGE"
#define MIRO_CamTriggerFilterString         "MIRO_TRIGGER_FILTER"
#define MIRO_CamReadySignalString           "MIRO_READY_SIGNAL"
#define MIRO_CamAuxPinString                "MIRO_AUX_PIN"

#define MIRO_LivePreviewString              "MIRO_LIVE_PREVIEW"
#define MIRO_SyncClockString                "MIRO_SYNC_CLOCK"

#define MIRO_CamQuietFanString              "MIRO_FAN_QUIET"

#define MIRO_AutoTriggerXString             "MIRO_AUTOTRIG_X"
#define MIRO_AutoTriggerYString             "MIRO_AUTOTRIG_Y"
#define MIRO_AutoTriggerWString             "MIRO_AUTOTRIG_W"
#define MIRO_AutoTriggerHString             "MIRO_AUTOTRIG_H"
#define MIRO_AutoTriggerThresholdString     "MIRO_AUTOTRIG_THRESH"
#define MIRO_AutoTriggerAreaString          "MIRO_AUTOTRIG_AREA"
#define MIRO_AutoTriggerIntervalString      "MIRO_AUTOTRIG_INTERVAL"
#define MIRO_AutoTriggerModeString          "MIRO_AUTOTRIG_MODE"

#define OLDMAXFILENAME 65 // maximum file path size for the continuous recording
// to keep compatibility with old setup files
#define MAXLENDESCRIPTION_OLD 121//maximum length for setup description
//(before Phantom 638)
#define MAXLENDESCRIPTION 4096 // maximum length for new setup description

#define MAXSTDSTRSZ 256

  typedef int bool32_t;

  //Time code according to the standard SMPTE 12M-1999
  typedef struct tagTC {
    uint8_t framesU:4; // Units of frames
    uint8_t framesT:2; // Tens of frames
    uint8_t dropFrameFlag:1; // Dropframe flag
    uint8_t colorFrameFlag:1; // Colorframe flag
    uint8_t secondsU:4; // Units of seconds
    uint8_t secondsT:3; // Tens of seconds
    uint8_t flag1:1; // Flag 1
    uint8_t minutesU:4; // Units of minutes
    uint8_t minutesT:3; // Tens of minutes
    uint8_t flag2:1; // Flag 2
    uint8_t hoursU:4; // Units of hours
    uint8_t hoursT:2; // Tens of hours
    uint8_t flag3:1; // Flag 3
    uint8_t flag4:1; // Flag 4
    uint32_t userBitData; // 32 user bits
  } TC, *PTC;

  //Color channels adjustment
  //intended for the White balance adjustment on color camera
  //by changing the gains of the red and blue channels
  typedef struct tagWBGAIN {
    float R; //White balance, gain correction for red
    float B; //White balance, gain correction for blue
  } WBGAIN, *PWBGAIN;

  typedef struct tagRECT {
    int32_t left;
    int32_t top;
    int32_t right;
    int32_t bottom;
  } RECT, *PRECT;

  // Image processing: Filtering
  typedef struct tagIMFILTER {
    int32_t dim; //square kernel dimension 3,5
    int32_t shifts; //right shifts of Coef (8 shifts means divide by 256)
    int32_t bias; //bias to add at end
    int32_t Coef[5*5]; //maximum alocation for a 5x5 filter
  } IMFILTER, *PIMFILTER;

  typedef struct tagCINEFILEHEADER {
    uint16_t Type;
    uint16_t Headersize;
    uint16_t Compression;
    uint16_t Version;
    int32_t FirstMovieImage;
    uint32_t TotalImageCount;
    int32_t FirstImageNo;
    uint32_t ImageCount;
    uint32_t OffImageHeader;
    uint32_t OffSetup;
    uint32_t OffImageOffsets;
    uint64_t TriggerTime;
  } CINEFILEHEADER;

  typedef struct tagBITMAPINFOHEADER {
    uint32_t biSize;
    int32_t biWidth;
    int32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t biXPelsPerMeter;
    int32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
  } BITMAPINFOHEADER;

  typedef struct tagSETUP {
    uint16_t FrameRate16;
    uint16_t Shutter16;
    uint16_t PostTrigger16;
    uint16_t FrameDelay16;
    uint16_t AspectRatio;
    uint16_t Res7;
    uint16_t Res8;
    uint8_t Res9;
    uint8_t Res10;
    uint8_t Res11;
    uint8_t TrigFrame;
    uint8_t Res12;
    char DescriptionOld[MAXLENDESCRIPTION_OLD];
    uint16_t Mark;
    uint16_t Length;
    uint16_t Res13;
    uint16_t SigOption;
    int16_t BinChannels;
    uint8_t SamplesPerImage;
    char BinName[8][11];
    uint16_t AnaOption;
    int16_t AnaChannels;
    uint8_t Res6;
    uint8_t AnaBoard;
    int16_t ChOption[8];
    float AnaGain[8];
    char AnaUnit[8][6];
    char AnaName[8][11];
    int32_t lFirstImage;
    uint32_t dwImageCount;
    int16_t nQFactor;
    uint16_t wCineFileType;
    char szCinePath[4][OLDMAXFILENAME];
    uint16_t Res14;
    uint8_t Res15;
    uint8_t Res16;
    uint16_t Res17;
    double Res18;
    double Res19;
    uint16_t Res20;
    int32_t Res1;
    int32_t Res2;
    int32_t Res3;
    uint16_t ImWidth;
    uint16_t ImHeight;
    uint16_t EDRShutter16;
    uint32_t Serial;
    int32_t Saturation;
    uint8_t Res5;
    uint32_t AutoExposure;
    bool32_t bFlipH;
    bool32_t bFlipV;
    uint32_t Grid;
    uint32_t FrameRate;
    uint32_t Shutter;
    uint32_t EDRShutter;
    uint32_t PostTrigger;
    uint32_t FrameDelay;
    bool32_t bEnableColor;
    uint32_t CameraVersion;
    uint32_t FirmwareVersion;
    uint32_t SoftwareVersion;
    int32_t RecordingTimeZone;
    uint32_t CFA;
    int32_t Bright;
    int32_t Contrast;
    int32_t Gamma;
    uint32_t Res21;
    uint32_t AutoExpLevel;
    uint32_t AutoExpSpeed;
    RECT AutoExpRect;
    WBGAIN WBGain[4];
    int32_t Rotate;
    WBGAIN WBView;
    uint32_t RealBPP;
    uint32_t Conv8Min;
    uint32_t Conv8Max;
    int32_t FilterCode;
    int32_t FilterParam;
    IMFILTER UF;
    uint32_t BlackCalSVer;
    uint32_t WhiteCalSVer;
    uint32_t GrayCalSVer;
    bool32_t bStampTime;
    uint32_t SoundDest;
    uint32_t FRPSteps;
    int32_t FRPImgNr[16];
    uint32_t FRPRate[16];
    uint32_t FRPExp[16];
    int32_t MCCnt;
    float MCPercent[64];
    uint32_t CICalib;
    uint32_t CalibWidth;
    uint32_t CalibHeight;
    uint32_t CalibRate;
    uint32_t CalibExp;
    uint32_t CalibEDR;
    uint32_t CalibTemp;
    uint32_t HeadSerial[4];
    uint32_t RangeCode;
    uint32_t RangeSize;
    uint32_t Decimation;
    uint32_t MasterSerial;
    uint32_t Sensor;
    uint32_t ShutterNs;
    uint32_t EDRShutterNs;
    uint32_t FrameDelayNs;
    uint32_t ImPosXAcq;
    uint32_t ImPosYAcq;
    uint32_t ImWidthAcq;
    uint32_t ImHeightAcq;
    char Description[MAXLENDESCRIPTION];
    bool32_t RisingEdge;
    uint32_t FilterTime;
    bool32_t LongReady;
    bool32_t ShutterOff;
    uint8_t Res4[16];
    bool32_t bMetaWB;
    int32_t Hue;
    int32_t BlackLevel;
    int32_t WhiteLevel;
    char LensDescription[256];
    float LensAperture;
    float LensFocusDistance;
    float LensFocalLength;
    float fOffset;
    float fGain;
    float fSaturation;
    float fHue;
    float fGamma;
    float fGammaR;
    float fGammaB;
    float fFlare;
    float fPedestalR;
    float fPedestalG;
    float fPedestalB;
    float fChroma;
    char ToneLabel[256];
    int32_t TonePoints;
    float fTone[32*2];
    char UserMatrixLabel[256];
    bool32_t EnableMatrices;
    float cmUser[9];
    bool32_t EnableCrop;
    RECT CropRect;
    bool32_t EnableResample;
    uint32_t ResampleWidth;
    uint32_t ResampleHeight;
    float fGain16_8;
    uint32_t FRPShape[16];
    TC TrigTC;
    float fPbRate;
    float fTcRate;
    char CineName[256];
    float fGainR;
    float fGainG;
    float fGainB;
    float cmCalib[9];
    float fWBTemp;
    float fWBCc;
    char CalibrationInfo[1024];
    char OpticalFilter[1024];
    char GpsInfo[MAXSTDSTRSZ];
    char Uuid[MAXSTDSTRSZ];
    char CreatedBy[MAXSTDSTRSZ];
    uint32_t RecBPP;
    uint16_t LowestFormatBPP;
    uint16_t LowestFormatQ;
  } SETUP;

  typedef struct tagINFOBLOCK {
    uint32_t size;
    uint16_t type;
    uint16_t reserved;
  } INFOBLOCK;

  typedef struct tagTIME64 {
    uint32_t frac;
    uint32_t secs;
  } TIME64;

  struct short_time_stamp32
  {
    // cam.tsformat = 1
    // time from beginning of the year in 1/100 sec units
    unsigned int csecs;
    // exposure time in us
    unsigned short exptime;
    // bits[15..2]: fractions (us to 10000); b[1]:event; b[0]:lock
    unsigned short frac;
    // exposure extension up to 32 bits
    unsigned short exptime32;
    // fractions extension up to 32 bits
    unsigned short frac32;
  };

typedef enum
{
  MIROTypeFloat,
  MIROTypeInteger,
  MIROTypeString,
  MIROTypeHex,
  MIROTypeRes,
  MIROTypeFlags,
  MIROTypeUnknown
} MIROValueType_t;

class miroVal
{ // Define class which contains data item value and type
private:
  std::string value;
  MIROValueType_t type;
public:
  miroVal() : value(""),
              type(MIROTypeFloat)
    {  }
  friend bool  operator<(const miroVal&, const miroVal&);
  friend bool operator==(const miroVal&, const miroVal&);
// accessor methods
  std::string getValue() {return this->value;}
  MIROValueType_t getType() {return this->type;}
  void setValue(std::string& s) {this->value = s;}
  void setType(MIROValueType_t t) {this->type = t;}
}; // end Class miroVal

class MiroMeta
{
public:
  std::string name_;
  std::string desc_;
  std::string param_;
  NDAttrDataType_t type_;
  int offset_;
  int size_;
  std::string sval_;
  int ival_;
  char cval_;
  double dval_;
  void *vPtr_;

  MiroMeta(const std::string& name,
      const std::string& desc,
      const std::string& param,
      NDAttrDataType_t type,
      int offset = 0,
      int size = 0) :
        name_(name),
        desc_(desc),
        param_(param),
        type_(type),
        offset_(offset),
        size_(size),
        sval_(""),
        ival_(0),
        cval_(0),
        dval_(0.0)
  {
    if (size_ > 0){
      vPtr_ = malloc(size_);
    }
  }

  void setStringVal(const std::string& sval)
  {
    sval_ = sval;
    if (type_ == NDAttrInt8){
      std::stringstream integerValueStream(sval_);
      integerValueStream >> cval_;
    } else if (type_ == NDAttrInt32){
      std::stringstream integerValueStream(sval_);
      integerValueStream >> ival_;
    } else if (type_ == NDAttrFloat64){
      std::stringstream doubleValueStream(sval_);
      doubleValueStream >> dval_;
    }
  }
};

class MiroCamera: public ADDriver
{
  public:
    MiroCamera(const char *portName, const char *ctrlPort, const char *dataPort, int maxBuffers, size_t maxMemory, int priority, int stackSize);
    virtual ~MiroCamera();
    void miroCameraTask();
    void miroStatusTask();
    void miroPreviewTask();
    void miroFlashTask();
    asynStatus makeConnection();
    asynStatus connect();
    asynStatus disconnect();
//    asynStatus readEnum(asynUser *pasynUser, char *strings[], int values[], int severities[], size_t nElements, size_t *nIn);
    asynStatus writeInt32(asynUser *pasynUser, epicsInt32 value);
    asynStatus writeFloat64(asynUser *pasynUser, epicsFloat64 value);
    asynStatus writeOctet(asynUser *pasynUser, const char *value, size_t maxChars, size_t *nActual);
    asynStatus sendSimpleCommand(const std::string& command, std::string *data = NULL, double timeout=MIRO_TIMEOUT);
    asynStatus setCameraResolution();
    asynStatus performCSR();
    asynStatus attachToPort(const std::string& portName);
    asynStatus readoutPreviewData();
    asynStatus sendSoftwareTrigger();
    asynStatus downloadCineFile(int cine);
    asynStatus saveCineToFlash(int cine);
    asynStatus saveSettings();
    asynStatus loadSettings();
    asynStatus formatFlash();
    asynStatus deleteFlashFile();
    asynStatus selectFlashByIndex(int index);
    asynStatus downloadFlashFile();
    asynStatus downloadFlashHeader(const std::string& filename);
    asynStatus downloadFlashImages(const std::string& filename, int start, int end);
    asynStatus convert10BitPacketTo12Bit(void *input, void *output);
    asynStatus readoutTimestamps(int cine, int start, int end);
    asynStatus readoutDataStream(int cine, int start, int end);
    asynStatus readFrame(int bytes);
    asynStatus updatePreviewCine();
    asynStatus updateCine(int cine);
    asynStatus selectCine(int cine);
    asynStatus setPartition(int count);
    asynStatus updateFlash();
    asynStatus updateFlashFileTable();
    asynStatus updateAutoStatus();
    asynStatus updateInfoStatus();
    asynStatus updateCameraStatus();
    asynStatus updateDefcStatus();
    asynStatus updateMetaStatus();
    asynStatus updateIntegerParameter(const std::string& name, int paramID);
    asynStatus updateStringParameter(const std::string& name, int paramID);
    asynStatus getCameraDataStruc(const std::string& strucName, std::map<std::string, miroVal>& params);
    asynStatus getCameraParameterType(const std::string& name, MIROValueType_t &value);
    asynStatus getCameraParameter(const std::string& name, int &value);
    asynStatus getCameraParameter(const std::string& name, double &value);
    asynStatus getCameraParameter(const std::string& name, std::string &value);
    asynStatus getCameraParameter(const std::string& name, bool &value);
    asynStatus setCameraParameter(const std::string& name, int value);
    asynStatus setCameraParameter(const std::string& name, double value);
    asynStatus setCameraParameter(const std::string& name, std::string value);
    asynStatus cineStates(const std::string& states, int &mask);
    bool checkState(const std::string &state, const std::string &item);
    asynStatus stringToInteger(const std::string& name, int &value);
    asynStatus stringToDouble(const std::string& name, double &value);
    asynStatus readIntegerData(std::map<std::string, std::string> data, const std::string& name, int &value);
    asynStatus readDoubleData(std::map<std::string, std::string> data, const std::string& name, double &value);
    asynStatus readRunModes();
    int getLocalPortNum(const std::string& driverPort);
    asynStatus asynPortConnect(const char *port, int addr, asynUser **ppasynUser, const char *inputEos, const char *outputEos);
    asynStatus asynPortDisconnect(asynUser *pasynUser);
    asynStatus commandResponse(const std::string &command, std::string *response, double timeout=MIRO_TIMEOUT);
    asynStatus asynWriteRead(const char *command, char *response, double timeout=MIRO_TIMEOUT);

    // String cleanup routines
    asynStatus cleanString(std::string &str, const std::string &search = ": \n", int where = 0);

    // Debugging routines
    asynStatus initDebugger(int initDebug);
    asynStatus debugLevel(const std::string& method, int onOff);
    asynStatus debug(const std::string& method, const std::string& msg);
    asynStatus debug(const std::string& method, const std::string& msg, int value);
    asynStatus debug(const std::string& method, const std::string& msg, double value);
    asynStatus debug(const std::string& method, const std::string& msg, const std::string& value);
    asynStatus debug(const std::string& method, const std::string& msg, std::map<std::string, std::string> value);

  protected:
    int MIROConnect_;
    #define FIRST_MIRO_PARAM MIROConnect_
    int MIRO_AcquireState_;
    int MIRO_SoftwareTrigger_;
    int MIRO_PerformCSR_;
    int MIRO_CSRCount_;
    int MIRO_SettingsSlot_;
    int MIRO_SettingsSave_;
    int MIRO_SettingsLoad_;
    int MIRO_AutoSave_;
    int MIRO_AutoRestart_;
    int MIRO_AutoCSR_;
    int MIRO_PostTrigCount_;
    int MIRO_TotalFrameCount_;
    int MIRO_MaxFrameCount_;
    int MIRO_SelectedCine_;
    int MIRO_CineName_;
		int MIRO_CineWidth_;
		int MIRO_CineHeight_;
    int MIRO_CineFrameCount_;
    int MIRO_CineFirstFrame_;
    int MIRO_CineLastFrame_;
    int MIRO_LivePreview_;
    int MIRO_CineRecordStart_;
    int MIRO_CineRecordEnd_;
    int MIRO_CineRecord_;
    int MIRO_CineRecordCount_;
    int MIRO_CineSaveCF_;
    int MIRO_SetPartition_;
    int MIRO_GetCineCount_;
    int MIRO_CFState_;
    int MIRO_CFAction_;
    int MIRO_CFSize_;
    int MIRO_CFUsed_;
    int MIRO_CFProgress_;
    int MIRO_CFError_;
    int MIRO_CFNumFiles_;
    int MIRO_CFMaxPages_;
    int MIRO_CFCurPage_;
    int MIRO_CFFormat_;
    int MIRO_CFFileName_;
    int MIRO_CFSIndex_;
    int MIRO_CFSWidth_;
    int MIRO_CFSHeight_;
    int MIRO_CFSFrameCount_;
    int MIRO_CFSFirstFrame_;
    int MIRO_CFSLastFrame_;
    int MIRO_CFSRecordStart_;
    int MIRO_CFSRecordEnd_;
    int MIRO_CFSRecord_;
    int MIRO_CFSRecordCount_;
    int MIRO_CFSFileDelete_;
    int MIRO_InfoSensorTemp_;
    int MIRO_InfoThermoPower_;
    int MIRO_InfoCameraTemp_;
    int MIRO_InfoFanPower_;
    int MIRO_EDR_;
    int MIRO_CamExtSync_;
    int MIRO_CamFrameDelay_;
    int MIRO_CamTriggerEdge_;
    int MIRO_CamTriggerFilter_;
    int MIRO_CamReadySignal_;
    int MIRO_CamQuietFan_;
    int MIRO_CamAuxPin_;
    int MIRO_AutoTriggerX_;
    int MIRO_AutoTriggerY_;
    int MIRO_AutoTriggerW_;
    int MIRO_AutoTriggerH_;
    int MIRO_AutoTriggerThreshold_;
    int MIRO_AutoTriggerArea_;
    int MIRO_AutoTriggerInterval_;
    int MIRO_AutoTriggerMode_;
    int MIRO_CnName_[MIRO_NUMBER_OF_CINES];
    int MIRO_CnWidth_[MIRO_NUMBER_OF_CINES];
    int MIRO_CnHeight_[MIRO_NUMBER_OF_CINES];
    int MIRO_CnStatus_[MIRO_NUMBER_OF_CINES];
    int MIRO_CnFrameCount_[MIRO_NUMBER_OF_CINES];
    int MIRO_CnFirstFrame_[MIRO_NUMBER_OF_CINES];
    int MIRO_CnLastFrame_[MIRO_NUMBER_OF_CINES];
    int MIRO_CnRecordStart_[MIRO_NUMBER_OF_CINES];
    int MIRO_CnRecordEnd_[MIRO_NUMBER_OF_CINES];
    int MIRO_CnRecord_[MIRO_NUMBER_OF_CINES];
    int MIRO_CnRecordCount_[MIRO_NUMBER_OF_CINES];
    int MIRO_CfFileName_[MIRO_NUMBER_OF_FLASH_FILES];
    int MIRO_CfFileSize_[MIRO_NUMBER_OF_FLASH_FILES];
    int MIRO_CfFileDate_[MIRO_NUMBER_OF_FLASH_FILES];
    int MIROConnected_;
    int MIRO_SyncClock;
    #define LAST_MIRO_PARAM MIROConnected_

  private:
    static const int MIRO_LinLUT[1024];
    static const char *MIRO_CnNameString[];
    static const char *MIRO_CnWidthString[];
    static const char *MIRO_CnHeightString[];
    static const char *MIRO_CnStatusString[];
    static const char *MIRO_CnFrameCountString[];
    static const char *MIRO_CnFirstFrameString[];
    static const char *MIRO_CnLastFrameString[];
    static const char *MIRO_CnRecordStartString[];
    static const char *MIRO_CnRecordEndString[];
    static const char *MIRO_CnRecordString[];
    static const char *MIRO_CnRecordCountString[];
    static const char *MIRO_CfFileNameString[];
    static const char *MIRO_CfFileSizeString[];
    static const char *MIRO_CfFileDateString[];

    asynUser                           *portUser_;
    asynUser                           *dataChannel_;
    char                               ctrlPort_[128];
    char                               dataPort_[128];
    char                               data_[2048000];
    char                               imgData_[2048000];
    char                               flashData_[2048000];
    std::vector<short_time_stamp32>    timestampData_;
    std::vector<tagTIME64>             flashTsData_;
    std::vector<uint32_t>              flashExpData_;
    std::vector<uint8_t>               flashIrigData_;
    std::vector<uint8_t>               flashEventData_;
    int                                flashTrigSecs_;
    int                                flashTrigUsecs_;
    int                                previewWidth_;
    int                                previewHeight_;
    std::map<std::string, int>         debugMap_;
    epicsEventId                       startEventId_;
    epicsEventId                       stopEventId_;
    epicsEventId                       startPreviewEventId_;
    epicsEventId                       stopPreviewEventId_;
    epicsEventId                       flashEventId_;
    std::vector<MiroMeta *>            metaArray_;
    std::vector<std::string>           lensModes_;
    std::vector<std::string>           scanRanges_;
    std::vector<std::string>           runModes_;
    std::map<std::string, miroVal> paramMap_;
    std::map<int, std::string>         paramIndexes_;
    std::vector<std::vector<std::string> > fileInfoSet_;
    bool                               firstConnect_;
    CINEFILEHEADER                     cineHeader_;
    BITMAPINFOHEADER                   cineBitmapHeader_;
    SETUP                              cineSetupHeader_;

};

// Number of asyn parameters (asyn commands) this driver supports
// Currently hardcoding 100 additional device specific parameters
#define NUM_MIRO_PARAMS ((int)(&LAST_MIRO_PARAM - &FIRST_MIRO_PARAM + 100))

std::string fileToString(const std::string fname);
std::string stripControl(std::string& , const char* ws = "\\\t\n\r\f\v" );
std::string deleteParen(std::string& str);
void parseDataStruc(const std::string&, std::vector<std::string>& strucName, std::vector<miroVal>& value);
