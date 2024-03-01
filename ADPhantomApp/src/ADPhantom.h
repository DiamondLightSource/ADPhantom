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
#include <pthread.h>

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
#include "asynCommonSyncIO.h"

// libpcap include
#include <pcap/pcap.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>

// Phantom camera PH16 protocol data definitions
#include <ph16UnitStructure.h>


//Required for 10gig connection
#ifndef ETHERTYPE_IEEE802A
#define ETHERTYPE_IEEE802A 0x88B7
#endif

//Define 2GB in bytes
#define TWO_GB_IN_BYTES 2147483648

// Protocol buffer size is 64K
#define PHANTOM_MAX_STRING 8192
// Asyn timeout
#define PHANTOM_TIMEOUT 1.000
#define PHANTOM_EXTENDED_TIMEOUT 200.0 // Format time out
// PHANTOM Update rate
#define PHANTOM_UPDATE_RATE 0.1
// PHANTOM Response OK/ERROR
#define PHANTOM_OK_STRING    "Ok!"
#define PHANTOM_ERROR_STRING "ERR:"

// Number of Conversion Threads
#define PHANTOM_CONV_THREADS 10

// PHANTOM Run Modes
#define PHANTOM_RUN_FAT  0
#define PHANTOM_RUN_SFAT 1
#define PHANTOM_RUN_FRR  2
#define PHANTOM_RUN_FE   3

// PHANTOM Cine States
#define PHANTOM_CINE_STATE_INV 1
#define PHANTOM_CINE_STATE_STR 2
#define PHANTOM_CINE_STATE_WTR 4
#define PHANTOM_CINE_STATE_TRG 8
#define PHANTOM_CINE_STATE_RDY 16
#define PHANTOM_CINE_STATE_DEF 32
#define PHANTOM_CINE_STATE_ABL 64
#define PHANTOM_CINE_STATE_PRE 128
#define PHANTOM_CINE_STATE_ACT 256
#define PHANTOM_CINE_STATE_REU 512

// PHANTOM Data Types
#define PHANTOM_TYPE_FLOAT   "float"
#define PHANTOM_TYPE_INTEGER "integer"
#define PHANTOM_TYPE_STRING  "string"
#define PHANTOM_TYPE_HEX     "hex"
#define PHANTOM_TYPE_RES     "resolution"
#define PHANTOM_TYPE_FLAGS   "flaglist"

// Number of Cines
#define PHANTOM_NUMBER_OF_CINES  64

// Number of flash files to display at one time
#define PHANTOM_NUMBER_OF_FLASH_FILES 8

// PHANTOM Command Strings
#define PHANTOM_CMD_GET_VALUE    "get"
#define PHANTOM_CMD_SET_VALUE    "set"
#define PHANTOM_CMD_ABORT        "rec 0"
#define PHANTOM_CMD_START_REC    "rec"
#define PHANTOM_CMD_DELETE       "del"
#define PHANTOM_CMD_RELEASE      "rel"
#define PHANTOM_CMD_TRIG         "trig"
#define PHANTOM_CMD_GET_CSTATES  "cstats"
#define PHANTOM_CMD_START_DATA   "startdata"
#define PHANTOM_CMD_ATTACH       "attach"
#define PHANTOM_CMD_GET_IMG      "img"
#define PHANTOM_CMD_GET_XIMG     "ximg"
#define PHANTOM_CMD_GET_TSTAMP   "time"
#define PHANTOM_CMD_PARTITION    "partition"
#define PHANTOM_CMD_WHITEBAL     "wbal"
#define PHANTOM_CMD_WUPDATE      "wupdate"
#define PHANTOM_CMD_BLACKREF     "bref"
#define PHANTOM_CMD_BUPDATE      "bupdate"
#define PHANTOM_CMD_FERASE       "ferase"
#define PHANTOM_CMD_FSAVE        "fsave"
#define PHANTOM_CMD_CFSAVE       "cfsave"
#define PHANTOM_CMD_GET_LOG      "tail"
#define PHANTOM_CMD_VPLAY        "vplay"
#define PHANTOM_CMD_CHECKCLEAN   "clean"
#define PHANTOM_CMD_ENABNOTIFY   "notify"
#define PHANTOM_CMD_USERSAVE     "usave"
#define PHANTOM_CMD_USERLOAD     "uload"
#define PHANTOM_CMD_USERERASE    "uerase"
#define PHANTOM_CMD_USERLIST     "uls"
#define PHANTOM_CMD_DEBUG        "console"
#define PHANTOM_CMD_SET_FSTOP    "fstop"
#define PHANTOM_CMD_FOCUS        "focus"
#define PHANTOM_CMD_CALIB        "calib"
#define PHANTOM_CMD_SYSMON       "sysmon"
#define PHANTOM_CMD_TESTIMG      "testimg"
#define PHANTOM_CMD_SETRTC       "setrtc"
#define PHANTOM_CMD_CFLS         "cfls"
#define PHANTOM_CMD_CFRM         "cfrm"
#define PHANTOM_CMD_CFFORMAT     "cfformat"
#define PHANTOM_CMD_CFREAD       "cfread"
#define PHANTOM_CMD_PRESET       "preset"
#define PHANTOM_CMD_MMSET        "mmset"

// Pre-defined EPICS Parameter Names
#define PHANTOMConnectString                   "PHANTOM_CONNECT"
#define PHANTOMConnectedString                 "PHANTOM_CONNECTED"

#define PHANTOM_AcquireStateString             "PHANTOM_ACQUIRE_STATE"

#define PHANTOM_SoftwareTriggerString          "PHANTOM_SOFTWARE_TRIGGER"
#define PHANTOM_PerformCSRString               "PHANTOM_PERFORM_CSR"
#define PHANTOM_CSRCountString                 "PHANTOM_CSR_COUNT"

#define PHANTOM_SettingsSlotString             "PHANTOM_SETTINGS_SLOT"
#define PHANTOM_SettingsSaveString             "PHANTOM_SETTINGS_SAVE"
#define PHANTOM_SettingsLoadString             "PHANTOM_SETTINGS_LOAD"

#define PHANTOM_AutoAdvanceString             "PHANTOM_AUTO_ADVANCE"

#define PHANTOM_AutoSaveString                 "PHANTOM_AUTO_SAVE"
#define PHANTOM_AutoRestartString              "PHANTOM_AUTO_RESTART"
#define PHANTOM_AutoCSRString                  "PHANTOM_AUTO_CSR"

#define PHANTOM_PostTrigCountString            "PHANTOM_POST_TRIG_COUNT"
#define PHANTOM_TotalFrameCountString          "PHANTOM_TOTAL_FRAME_COUNT"
#define PHANTOM_MaxFrameCountString            "PHANTOM_MAX_FRAME_COUNT"
#define PHANTOM_FramesPerSecondString          "PHANTOM_FRAMES_PER_SECOND"

#define PHANTOM_CineNameString                 "PHANTOM_CINE_NAME"
#define PHANTOM_SelectedCineString             "PHANTOM_CINE_SELECTED"
#define PHANTOM_CineWidthString                "PHANTOM_CINE_WIDTH"
#define PHANTOM_CineHeightString               "PHANTOM_CINE_HEIGHT"
#define PHANTOM_CineFrameCountString           "PHANTOM_CINE_FR_COUNT"
#define PHANTOM_CineFirstFrameString           "PHANTOM_CINE_FIRST_FR"
#define PHANTOM_CineLastFrameString            "PHANTOM_CINE_LAST_FR"
#define PHANTOM_DownloadStartFrameString       "PHANTOM_DOWNLOAD_START_FRAME"
#define PHANTOM_DownloadEndFrameString         "PHANTOM_DOWNLOAD_END_FRAME"
#define PHANTOM_DownloadStartCineString        "PHANTOM_DOWNLOAD_START_CINE"
#define PHANTOM_DownloadEndCineString          "PHANTOM_DOWNLOAD_END_CINE"
#define PHANTOM_DownloadString                 "PHANTOM_DOWNLOAD"
#define PHANTOM_DownloadAbortString            "PHANTOM_DOWNLOAD_ABORT"
#define PHANTOM_DownloadCountString            "PHANTOM_DOWNLOAD_COUNT"
#define PHANTOM_DownloadFrameModeString        "PHANTOM_DOWNLOAD_FRAME_MODE"
#define PHANTOM_DownloadSpeedString            "PHANTOM_DOWNLOAD_SPEED"
#define PHANTOM_DroppedPacketsString           "PHANTOM_DROPPED_PACKETS"
#define PHANTOM_MarkCineSavedString            "PHANTOM_MARK_CINE_SAVED"
#define PHANTOM_CineSaveCFString               "PHANTOM_CINE_SAVE_CF"       // Save selected cine to flash

#define PHANTOM_DeleteString                   "PHANTOM_DELETE"
#define PHANTOM_DeleteStartCineString          "PHANTOM_DELETE_START_CINE"
#define PHANTOM_DeleteEndCineString            "PHANTOM_DELETE_END_CINE"

#define PHANTOM_SetPartitionString             "PHANTOM_SET_PARTITION"
#define PHANTOM_GetCineCountString             "PHANTOM_GET_CINE_COUNT"

#define PHANTOM_DataFormatString               "PHANTOM_DataFormat"   // Integer corresponding to format token (8,8R,P16,P16R,P10,P12L)

#define PHANTOM_CFStateString                  "PHANTOM_CF_STATE"
#define PHANTOM_CFActionString                 "PHANTOM_CF_ACTION"
#define PHANTOM_CFSizeString                   "PHANTOM_CF_SIZE"
#define PHANTOM_CFUsedString                   "PHANTOM_CF_USED"
#define PHANTOM_CFProgressString               "PHANTOM_CF_PROGRESS"
#define PHANTOM_CFErrorString                  "PHANTOM_CF_ERROR"
#define PHANTOM_CFNumFilesString               "PHANTOM_CF_NUM_FILES"
#define PHANTOM_CFMaxPagesString               "PHANTOM_CF_MAX_PAGES"
#define PHANTOM_CFCurPageString                "PHANTOM_CF_CUR_PAGE"
#define PHANTOM_CFFormatString                 "PHANTOM_CF_FORMAT"

#define PHANTOM_CFFileNameString               "PHANTOM_CF_FILENAME"
#define PHANTOM_CFSIndexString                 "PHANTOM_CFS_INDEX"
#define PHANTOM_CFSWidthString                 "PHANTOM_CFS_WIDTH"
#define PHANTOM_CFSHeightString                "PHANTOM_CFS_HEIGHT"
#define PHANTOM_CFSFrameCountString            "PHANTOM_CFS_FR_COUNT"
#define PHANTOM_CFSFirstFrameString            "PHANTOM_CFS_FIRST_FR"
#define PHANTOM_CFSLastFrameString             "PHANTOM_CFS_LAST_FR"
#define PHANTOM_CFSRecordStartString           "PHANTOM_CFS_REC_START"
#define PHANTOM_CFSRecordEndString             "PHANTOM_CFS_REC_END"
#define PHANTOM_CFSRecordString                "PHANTOM_CFS_RECORD"
#define PHANTOM_CFSRecordCountString           "PHANTOM_CFS_REC_COUNT"
#define PHANTOM_CFSFileDeleteString            "PHANTOM_CFS_DELETE"         // Delete the (C)ompact (F)lash (S)elected file

#define PHANTOM_InfoSensorTempString           "PHANTOM_SENSOR_TEMP"
#define PHANTOM_InfoThermoPowerString          "PHANTOM_THERMO_POWER"
#define PHANTOM_InfoCameraTempString           "PHANTOM_CAMERA_TEMP"
#define PHANTOM_InfoFanPowerString             "PHANTOM_FAN_POWER"
#define PHANTOM_EDRString                      "PHANTOM_EDR"
#define PHANTOM_CamExtSyncString               "PHANTOM_EXT_SYNC"
#define PHANTOM_CamFrameDelayString            "PHANTOM_FRAME_DELAY"
#define PHANTOM_CamTriggerEdgeString           "PHANTOM_TRIGGER_EDGE"
#define PHANTOM_CamTriggerFilterString         "PHANTOM_TRIGGER_FILTER"
#define PHANTOM_CamReadySignalString           "PHANTOM_READY_SIGNAL"
#define PHANTOM_CamAux1PinString               "PHANTOM_AUX1_PIN"
#define PHANTOM_CamAux2PinString               "PHANTOM_AUX2_PIN"
#define PHANTOM_CamAux4PinString               "PHANTOM_AUX4_PIN"

#define PHANTOM_LivePreviewString              "PHANTOM_LIVE_PREVIEW"
#define PHANTOM_SyncClockString                "PHANTOM_SYNC_CLOCK"

#define PHANTOM_CamQuietFanString              "PHANTOM_FAN_QUIET"

#define PHANTOM_AutoTriggerXString             "PHANTOM_AUTOTRIG_X"
#define PHANTOM_AutoTriggerYString             "PHANTOM_AUTOTRIG_Y"
#define PHANTOM_AutoTriggerWString             "PHANTOM_AUTOTRIG_W"
#define PHANTOM_AutoTriggerHString             "PHANTOM_AUTOTRIG_H"
#define PHANTOM_AutoTriggerThresholdString     "PHANTOM_AUTOTRIG_THRESH"
#define PHANTOM_AutoTriggerAreaString          "PHANTOM_AUTOTRIG_AREA"
#define PHANTOM_AutoTriggerIntervalString      "PHANTOM_AUTOTRIG_INTERVAL"
#define PHANTOM_AutoTriggerModeString          "PHANTOM_AUTOTRIG_MODE"

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
  PHANTOMTypeFloat,
  PHANTOMTypeInteger,
  PHANTOMTypeString,
  PHANTOMTypeHex,
  PHANTOMTypeRes,
  PHANTOMTypeFlags,
  PHANTOMTypeUnknown
} PHANTOMValueType_t;

class phantomVal
{ // Define class which contains data item value and type
private:
  std::string value;
  PHANTOMValueType_t type;
public:
  phantomVal() : value(""),
              type(PHANTOMTypeFloat)
    {  }
  friend bool  operator<(const phantomVal&, const phantomVal&);
  friend bool operator==(const phantomVal&, const phantomVal&);
// accessor methods
  std::string getValue() {return this->value;}
  PHANTOMValueType_t getType() {return this->type;}
  void setValue(std::string& s) {this->value = s;}
  void setType(PHANTOMValueType_t t) {this->type = t;}
}; // end Class phantomVal

class PhantomMeta
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

  PhantomMeta(const std::string& name,
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

class ADPhantom: public ADDriver
{
  public:
    ADPhantom(const char *portName, const char *ctrlPort, const char *dataPort, const char *macAddress, const char *interface, int maxBuffers, size_t maxMemory, int priority, int stackSize);
    virtual ~ADPhantom();
    void phantomCameraTask();
    void phantomStatusTask();
    void phantomPreviewTask();
    void phantomFlashTask();
    void phantomDownloadTask();
    void phantomConversionTask();
    asynStatus makeConnection();
    asynStatus connect();
    asynStatus disconnect();
//    asynStatus readEnum(asynUser *pasynUser, char *strings[], int values[], int severities[], size_t nElements, size_t *nIn);
    asynStatus writeInt32(asynUser *pasynUser, epicsInt32 value);
    asynStatus writeFloat64(asynUser *pasynUser, epicsFloat64 value);
    asynStatus writeOctet(asynUser *pasynUser, const char *value, size_t maxChars, size_t *nActual);
    asynStatus sendSimpleCommand(const std::string& command, std::string *data = NULL, double timeout=PHANTOM_TIMEOUT);
    asynStatus setCameraResolution();
    asynStatus performCSR();
    asynStatus attachToPort(const std::string& portName);
    asynStatus readoutPreviewData();
    asynStatus sendSoftwareTrigger();
    asynStatus deleteCineFiles();
    asynStatus saveCineToFlash(int cine);
    asynStatus saveSettings();
    asynStatus loadSettings();
    asynStatus formatFlash();
    asynStatus deleteFlashFile();
    asynStatus selectFlashByIndex(int index);
    asynStatus downloadFlashFile();
    asynStatus downloadFlashHeader(const std::string& filename);
    asynStatus downloadFlashImages(const std::string& filename, int start, int end);
    asynStatus readoutTimestamps(int start_cine, int end_cine, int start_frame, int end_frame, bool uni_frame_lim);
    asynStatus readoutDataStream(int start_cine, int end_cine, int start_frame, int end_frame, bool uni_frame_lim);
    asynStatus convertPixelData(int nBytes);
    asynStatus convert12BitPacketTo16Bit(unsigned char *input, unsigned char *output);
    asynStatus convert10BitPacketTo16Bit(unsigned char *input, unsigned char *output);
    asynStatus convert8BitPacketTo16Bit(unsigned char *input, unsigned char *output, int nBytes);
    asynStatus readFrame(int bytes);
    asynStatus readFrame10G(int bytes, int frame_no, unsigned char & packet_id, int & missed_packets);
    asynStatus updatePreviewCine();
    asynStatus updateCine(int cine);
    asynStatus selectCine(int cine);
    asynStatus setPartition(int count);
    asynStatus updateFlash();
    asynStatus updateFlashFileTable();
    asynStatus updateAutoStatus();
    asynStatus updateAutoTrigPos();
    asynStatus updateInfoStatus();
    asynStatus updateCameraStatus();
    asynStatus updateDefcStatus();
    asynStatus updateMetaStatus();
    asynStatus updateIntegerParameter(const std::string& name, int paramID);
    asynStatus updateStringParameter(const std::string& name, int paramID);
    asynStatus getCameraDataStruc(const std::string& strucName, std::map<std::string, phantomVal>& params);
    asynStatus getCameraParameterType(const std::string& name, PHANTOMValueType_t &value);
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
    asynStatus commandResponse(const std::string &command, std::string *response, double timeout=PHANTOM_TIMEOUT);
    asynStatus asynWriteRead(const char *command, char *response, double timeout=PHANTOM_TIMEOUT);

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

    struct timespec frameStart_;
    struct timespec readStart_;

  protected:
    int PHANTOMConnect_;
    #define FIRST_PHANTOM_PARAM PHANTOMConnect_
    int PHANTOM_AcquireState_;
    int PHANTOM_SoftwareTrigger_;
    int PHANTOM_PerformCSR_;
    int PHANTOM_CSRCount_;
    int PHANTOM_SettingsSlot_;
    int PHANTOM_SettingsSave_;
    int PHANTOM_SettingsLoad_;
    int PHANTOM_AutoAdvance_;
    int PHANTOM_AutoSave_;
    int PHANTOM_AutoRestart_;
    int PHANTOM_AutoCSR_;
    int PHANTOM_PostTrigCount_;
    int PHANTOM_TotalFrameCount_;
    int PHANTOM_MaxFrameCount_;
    int PHANTOM_SelectedCine_;
    int PHANTOM_CineName_;
		int PHANTOM_CineWidth_;
		int PHANTOM_CineHeight_;
    int PHANTOM_CineFrameCount_;
    int PHANTOM_CineFirstFrame_;
    int PHANTOM_CineLastFrame_;
    int PHANTOM_LivePreview_;
    int PHANTOM_DownloadStartFrame_;
    int PHANTOM_DownloadEndFrame_;
    int PHANTOM_DownloadStartCine_;
    int PHANTOM_DownloadEndCine_;
    int PHANTOM_Download_;
    int PHANTOM_DownloadAbort_;
    int PHANTOM_DownloadCount_;
    int PHANTOM_DownloadFrameMode_;
    int PHANTOM_DownloadSpeed_;
    int PHANTOM_DroppedPackets_;
    int PHANTOM_MarkCineSaved_;
    int PHANTOM_CineSaveCF_;
    int PHANTOM_Delete_;
    int PHANTOM_DeleteStartCine_;
    int PHANTOM_DeleteEndCine_;
    int PHANTOM_SetPartition_;
    int PHANTOM_GetCineCount_;
    int PHANTOM_CFState_;
    int PHANTOM_CFAction_;
    int PHANTOM_CFSize_;
    int PHANTOM_CFUsed_;
    int PHANTOM_CFProgress_;
    int PHANTOM_CFError_;
    int PHANTOM_CFNumFiles_;
    int PHANTOM_CFMaxPages_;
    int PHANTOM_CFCurPage_;
    int PHANTOM_CFFormat_;
    int PHANTOM_CFFileName_;
    int PHANTOM_CFSIndex_;
    int PHANTOM_CFSWidth_;
    int PHANTOM_CFSHeight_;
    int PHANTOM_CFSFrameCount_;
    int PHANTOM_CFSFirstFrame_;
    int PHANTOM_CFSLastFrame_;
    int PHANTOM_CFSRecordStart_;
    int PHANTOM_CFSRecordEnd_;
    int PHANTOM_CFSRecord_;
    int PHANTOM_CFSRecordCount_;
    int PHANTOM_CFSFileDelete_;
    int PHANTOM_InfoSensorTemp_;
    int PHANTOM_InfoThermoPower_;
    int PHANTOM_InfoCameraTemp_;
    int PHANTOM_InfoFanPower_;
    int PHANTOM_EDR_;
    int PHANTOM_CamExtSync_;
    int PHANTOM_CamFrameDelay_;
    int PHANTOM_CamTriggerEdge_;
    int PHANTOM_CamTriggerFilter_;
    int PHANTOM_CamReadySignal_;
    int PHANTOM_CamQuietFan_;
    int PHANTOM_CamAux1Pin_;
    int PHANTOM_CamAux2Pin_;
    int PHANTOM_CamAux4Pin_;
    int PHANTOM_AutoTriggerX_;
    int PHANTOM_AutoTriggerY_;
    int PHANTOM_AutoTriggerW_;
    int PHANTOM_AutoTriggerH_;
    int PHANTOM_AutoTriggerThreshold_;
    int PHANTOM_AutoTriggerArea_;
    int PHANTOM_AutoTriggerInterval_;
    int PHANTOM_AutoTriggerMode_;
    int PHANTOM_CnName_[PHANTOM_NUMBER_OF_CINES];
    int PHANTOM_CnWidth_[PHANTOM_NUMBER_OF_CINES];
    int PHANTOM_CnHeight_[PHANTOM_NUMBER_OF_CINES];
    int PHANTOM_CnStatus_[PHANTOM_NUMBER_OF_CINES];
    int PHANTOM_CnFrameCount_[PHANTOM_NUMBER_OF_CINES];
    int PHANTOM_CnFirstFrame_[PHANTOM_NUMBER_OF_CINES];
    int PHANTOM_CnLastFrame_[PHANTOM_NUMBER_OF_CINES];
    int PHANTOM_CnRecordStart_[PHANTOM_NUMBER_OF_CINES];
    int PHANTOM_CnRecordEnd_[PHANTOM_NUMBER_OF_CINES];
    int PHANTOM_CnRecord_[PHANTOM_NUMBER_OF_CINES];
    int PHANTOM_CnRecordCount_[PHANTOM_NUMBER_OF_CINES];
    int PHANTOM_CfFileName_[PHANTOM_NUMBER_OF_FLASH_FILES];
    int PHANTOM_CfFileSize_[PHANTOM_NUMBER_OF_FLASH_FILES];
    int PHANTOM_CfFileDate_[PHANTOM_NUMBER_OF_FLASH_FILES];
    int PHANTOM_DataFormat_;
    int PHANTOMConnected_;
    int PHANTOM_FramesPerSecond_;
    int PHANTOM_SyncClock_;
    #define LAST_PHANTOM_PARAM PHANTOM_SyncClock_

  private:
    static const int PHANTOM_LinLUT[1024];
    static const char *PHANTOM_CnNameString[];
    static const char *PHANTOM_CnWidthString[];
    static const char *PHANTOM_CnHeightString[];
    static const char *PHANTOM_CnStatusString[];
    static const char *PHANTOM_CnFrameCountString[];
    static const char *PHANTOM_CnFirstFrameString[];
    static const char *PHANTOM_CnLastFrameString[];
    static const char *PHANTOM_CnRecordStartString[];
    static const char *PHANTOM_CnRecordEndString[];
    static const char *PHANTOM_CnRecordString[];
    static const char *PHANTOM_CnRecordCountString[];
    static const char *PHANTOM_CfFileNameString[];
    static const char *PHANTOM_CfFileSizeString[];
    static const char *PHANTOM_CfFileDateString[];

    asynUser                           *portUser_;
    asynUser                           *dataChannel_;
    asynUser                           *commonDataport_;
    pcap_t                             *handle_;
    struct pcap_pkthdr                 *header_;
    char                               error_buffer[256];
    char                               ctrlPort_[128];
    char                               dataPort_[128];
    char                               macAddress_[14];
    char                               interface_[128];
    char                               data_[2048000];
    char                               imgData_[2048000];
    char                               flashData_[2048000];
    char                               downloadData_[102400000]; // 50, 2MB images
    std::vector<short_time_stamp32>    timestampData_;
    std::vector<tagTIME64>             flashTsData_;
    std::vector<uint32_t>              flashExpData_;
    std::vector<uint8_t>               flashIrigData_;
    std::vector<uint8_t>               flashEventData_;
    int                                flashTrigSecs_;
    int                                flashTrigUsecs_;
    int                                previewWidth_;
    int                                previewHeight_;
    int                                bitDepth_;
    int                                conversionBitDepth_;
    int                                conversionBytes_;
    int                                downloadingFlag_;
    std::string                        phantomToken_;
    std::map<std::string, int>         debugMap_;
    epicsEventId                       startEventId_;
    epicsEventId                       stopEventId_;
    epicsEventId                       startDownloadEventId_;
    epicsEventId                       startPreviewEventId_;
    epicsEventId                       stopPreviewEventId_;
    epicsEventId                       flashEventId_;
    epicsEventId                       convStartEvt_[PHANTOM_CONV_THREADS];
    epicsEventId                       convFinishEvt_[PHANTOM_CONV_THREADS];
    std::vector<PhantomMeta *>         metaArray_;
    std::vector<std::string>           lensModes_;
    std::vector<std::string>           scanRanges_;
    std::vector<std::string>           runModes_;
    std::map<std::string, phantomVal>  paramMap_;
    std::map<int, std::string>         paramIndexes_;
    std::vector<std::vector<std::string> > fileInfoSet_;
    bool                               firstConnect_;
    CINEFILEHEADER                     cineHeader_;
    BITMAPINFOHEADER                   cineBitmapHeader_;
    SETUP                              cineSetupHeader_;

};

// Number of asyn parameters (asyn commands) this driver supports
// Currently hardcoding 100 additional device specific parameters
#define NUM_PHANTOM_PARAMS ((int)(&LAST_PHANTOM_PARAM - &FIRST_PHANTOM_PARAM + 100))

std::string fileToString(const std::string fname);
std::string stripControl(std::string& , const char* ws = "\\\t\n\r\f\v" );
std::string deleteParen(std::string& str);
void parseDataStruc(const std::string&, std::vector<std::string>& strucName, std::vector<phantomVal>& value);