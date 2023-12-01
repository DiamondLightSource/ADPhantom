/* 
  Data structures used by Phantom PH16 camera Protocol.
  Derived from document v16proto-2.3.pdf
  
  Author:  Philip Taylor Date:    6 September 2016   
*/

const std::string stringTokens[] = {
"bref",	   /* The camera supports internal CSR (black reference) operations. */
"atrig",   /* The camera has an image-based autotrigger system. */
"blk4",	   /* The nominal black level of raw corrected images is at 4/256 of full scale. */
"v444",	   /* The camera support 4:4:4 3G-SDI outputs. */
"shtr",	   /* The camera has a capping shutter installed. */
"lowexp",  /* The camera has the FAST option installed, allowing minimum exposures of under 1μs. */
"hqmode",  /* The camera supports the high-quality acquisition mode. */
"ximg",	   /* The camera supports the ximg command. */
"edr",	   /* Camera supports double-slope exposures (edr). */
"burst",   /* The camera supports frame burst mode. */
"genlock", /* The camera has a genlock system. */
"decimate", /* The camera allows frame decimation. */
"ramp",	   /* The camera allows frame rate ramping during recording. */
"aexp",	   /* The camera has an autoexposure system. */
"mag",	   /* The camera has a cinemag socket. */
"cf",	   /* The camera has a file storage device (CF card, SSD, etc) port. */
"earlyimg", /* The camera supports reading numbered frames as soon as a cine is triggered. */
"attach",  /* The camera supports the attach command and corresponding method of establishing data sockets. */
"notify",  /* The camera supports the notify command that enables asynchronous state change messages. */
"v4k",	  /* The camera support 4k video output on dual 3G-SDI outputs. */
"log",	  /* The camera support log modes for the video outputs. */
"dualp",  /* The camera supports dual video pipes. */
"aux2",	  /* The camera supports aux2mode. */
"mm24",	  /* The camera supports 24-axes multi-matrix color correction. */
"wide",	  /* The camera supports anamorphic desqueeze (video.widescreen). */
"audio"  /* The camera supports audio i/o. */
};	

   struct ADPhantomInfo
   {
   /* Sensor information */
   unsigned int sensor;        /* Type of sensor used */
   unsigned int snsversion;    /* Version number of the sensor */
   unsigned int cfa;           /* Type of sensor colour filter */
   unsigned int filter;        /* Additional fiter used */
   /* Versions and Identification */
   unsigned int hwver;         /* Current hardware version */ 
   unsigned int kernel;        /* Current kernel version */
   unsigned int swver;         /* Current firmware version */
   unsigned int xver;          /* Current FPGA version */
   std::string model;               /* Camera model */
   unsigned int pver;          /* Protocol version */
   unsigned int sver;          /* System (kernel/filesystem) release number */
   unsigned int fver;          /* Firmware release number */ 
   unsigned int serial;        /* Camera serial number */
   std::string name;                /* Camera name */
   /* Capabilities */
   std::string features;            /* Space-separated tokens showing supported camera features */
   std::string imgformats;          /* Image formats supported by img command */
   std::string videosystems;        /* Formats supported on the video output */
   unsigned int maxcines;      /* Maximum allowed number of cines */
   unsigned int xmax;          /* Maximum horizontal resolution (pixels) */
   unsigned int ymax;          /* Maximum vertical resolution (pixels) */
   unsigned int xinc;          /* Horizontal increment */
   unsigned int yinc;          /* Vertical increment */
   unsigned int winx;          /* Windows can start at multiples of winx from left sensor edge */
   unsigned int winy;          /* Windows can start at multiples of winy from top sensor edge */
   unsigned int kernsz;        /* Size of acquisition kernel (in bytes) */
   unsigned int memsz;         /* Size of installed memory in megabytes */
   unsigned int cinemem;       /* Memory available for cine storage in megabytes */
   unsigned int mdepths;       /* Bit mask defining available memory depths */
   unsigned int expdead;       /* Minimum interval between exposures */
   unsigned int minexp;        /* Minimum exposure time in ns. */
   unsigned int xblock;        /* Number of x pixels read in a clock cycle */
   unsigned int yblock;        /* Number of y pixels read in a clock cycle */ 
   unsigned int pixps;         /* Time to read one pixel in picoseconds */
   unsigned int rotps;         /* Row overhead time in picoseconds */
   unsigned int fotps;         /* Frame overhead time in picoseconds */
   unsigned int minfrate;      /* Minimum framerate */
   unsigned int maxfrate;      /* Maximum framerate */
   unsigned int tmodel;        /* Timing model used by camera */
   unsigned int magtp;         /* Max pixel thoughput in pixels per sec */
   unsigned int rtobyteps;     /* Duration of one byte in picoseconds */
   unsigned int rtopacket;     /* Size of a packet in bytes */
   unsigned int rtopacketovhead; /* Overhead per packet in bytes */
   unsigned int rtofrovhead;   /* Overhead per frame in bytes */
   unsigned int rto_channels;  /* Number of RTO channels */
   
   float basechroma;           /* Default chroma gain */
   unsigned int baseei;        /* Base Exposure Index, in ISO units */
   int snstemp;                /* Sensor die temperature. */
   int tepower;                /* Amount of power used by sensor thermoelectric cooler as a %age of full cooling power. */
   int camtemp;                /* Camera temp measured inside body. */
   int fanpower;               /* Fan speed as a %age of maximum speed. */
   int batti;                  /* Battery current in milliamps */
   unsigned int battv;         /* Battery voltage in millivolts */
   float vbatt;                /* Battery voltage in volts */
   unsigned int battstate;     /* Battery status */
   unsigned int batttimer;     /* Number of seconds until camera turns off */
   unsigned int genlockstat;   /* State of genlock system */
   };
   
   struct ADPhantomHw
   {
   };
   
   struct ADPhantomMeta
   {
   };
   
   struct ADPhantomCam
   {
   unsigned int syncimg;      /* Frame sync mode */
   unsigned int frdelay;      /* Delay between the sync reference time and start of exposure, in ns. */
   unsigned int rtoen;        /* 1= enable real time output, 0= disable */
   unsigned int rtotfr;       /* RTO test frame size in bytes */
   unsigned int membpp;       /* Bit depth into image memory */
   unsigned int trigpol;      /* Trigger polarity */
   unsigned int trigfilt;     /* Minimum time in µs the trigger signal has to be asserted */
   unsigned int startonacq;   /* When 1, the camera starts waiting for trigger mode; when 0, starts in preview mode */
   unsigned int aux1mode;     /* aux1 signal (pin 6): 0=strobe, 1=event, 2=memgate, 3=fsync */
   unsigned int aux2mode;     /* Function of the aux2 signal (pin 5): 0=ready, 1=strobe, 2=aes/ebu out */
   unsigned int tsformat;     /* Timestamp format used in future recordings */
   unsigned int tcmode;       /* Timecode mode (0=irig, 1=smpte) */
   unsigned int master;       /* Serial number of the sync master */
   int apoffdis;              /* Auto poweroff disable */
   unsigned int longready;    /* Ready inactive at end recording not trigger */
   unsigned int cines;        /* Number of recordable cines (partitions) */
   int dark;                  /* Close (1)/Open (0) the mechanical shutter */
   unsigned int tsetsns;      /* Sensor temperature set point */
   unsigned int tsetcam;      /* Camera temperature set point */
   unsigned int tz;           /* Difference in seconds between local time and UTC */
   };

   struct autoTrigger {     /* Image-based autotrigger data structure */
   int x;                   /* X coordinate of the center of the autotrigger region */
   int y;                   /* Y coordinate of the center of the autotrigger region */
   unsigned int w;          /* Width of the autotrigger region */
   unsigned int h;          /* Height of the autotrigger region */
   unsigned int threshold;  /* Change required for autotrigger */
   unsigned int area;       /* Percentage of autotrigger region that must be active  */
   unsigned int speed;      /* Number of frames between updates of reference memory  */
   unsigned int mode;       /* Operating mode of the autotrigger */
   };
   
   struct ADPhantomAuto
   {
   unsigned int videoplay;      /* play back cine immediately after the recording ends */
   unsigned int flashsave;      /* save cine to a cinemag */
   unsigned int filesave;       /* save cine to file */
   unsigned int acqrestart;     /* restart acquisition after automatic save */
   unsigned int bref;           /* new black reference (CSR) at the start of each recording */
   int firstframe;              /* first frame of the region to be saved and/or played */
   int lastframe;               /* last frame of the region to be saved and/or played */
   unsigned int loops;          /* the loop counter for video play back */
   unsigned int speed;          /* Speed of video play back */
   unsigned int progress;       /* number of frames still to do by the auto process */
   unsigned int bref_progress;  /* number of frames left for black reference */
   autoTrigger trigger;         /* Image-based autotrigger data structure */
   };
   
   struct ADPhantomEth
   {
   std::string ip;                /* IP address */ 
   std::string netmask;           /* Network mask */ 
   std::string broadcast;         /* Broadcast mask */ 
   std::string gateway;           /* Address of default gateway */ 
   unsigned int mtu;         /* MTU used by the camera */ 
   std::string xip;               /* IP address for the 10GE */ 
   std::string xnetmask;          /* Network mask for the 10GE */ 
   std::string xbroadcast;        /* Broadcast mask for the 10GE */ 
   };
   
   struct ADPhantomVideo
   {
   };
   
   struct ADPhantomIrig
   {
   };
   
   struct ADPhantomMag     /* Cinemag Structure */
   {
   unsigned int state;         /* Status of the cinemag subsystem (see below) */
   unsigned int progress;      /* Progress indicator for various operations  */
   unsigned int protect;       /* cinemag has the erase protect switch on  */
   unsigned int size;          /* Size of the attached cinemag in kB  */
   unsigned int used;          /* Amount of space used in the attached cinemag, in kB */
   unsigned int takes;         /* The number of cines stored to the attached cinemag */
   unsigned int version;       /* Cinemag firmware version */
   std::string id;                /* Textual description of device attached */
   unsigned int runstop;       /* Enable direct recording to cinemag */
   unsigned int type;          /* Identification code for device attached  */
   };

/* Mag.state values */
#define magStateUnplugged      0
#define magStatePowerOff       1
#define magStateInitialising   2
#define magStateScanning       3
#define magStateReady          4
#define magStateRecording      5
#define magStatePlayback       6
#define magStateVideoplay      7
#define magStateErase          8
#define magStateEraseRaw       9
#define magStateErrorPending  10
#define magStateError         11
#define magStateInsertNotCinemag 12
#define magStateInslotNotCinemag 13

   struct ADPhantomCf
   {
   };
   
   struct ADPhantomUsets
   {
   };
   
   struct defcMeta {
   int ox;    /* X offset of window from camera frame center */
   int oy;    /* Y offset of window from camera frame center */
   int w;     /* Window width in camera pixels */
   int h;     /* Window height in camera pixels */
   int ow;    /* Window width after software crop and scale in pixels */
   int oh;    /* Window height after software crop and scale in pixels */
   int crop;  /* Enable software crop */
/*   int oh;     Enable software scale (Already have 'oh': ERROR IN PROTOCOL MANUAL?) */
   };
       
   struct ADPhantomDefc
   {
    unsigned int res;         /* Image size in pixels */
    float rate;               /* Frame rate in pictures per second */
    unsigned int exp;         /* Exposure time, in ns  */
    unsigned int edrexp;      /* EDR exposure time in ns */
    unsigned int ptframes;    /* Number of post-trigger frames */
    unsigned int shoff;       /* Shutter off */
    std::string ramp;            /* Frame rate ramping specification std::string */
    unsigned int bcount;      /* Number of frames per burst */
    unsigned int bperiod;     /* Time interval in ns between burst frames */
    unsigned int hqenable;    /* Enable HQ acquisition mode */
    unsigned int decimation;  /* Ratio between frame rate going into RAM and rate output */
    unsigned int frcount;     /* Number of frames that can be stored in a cine */
    unsigned int frsize;      /* Amount of space required to store a frame */
    unsigned int aexpmode;    /* Autoexposure mode */
    float aexpcomp;           /* Autoexposure compensation in stops */
    defcMeta meta;
   };
   
   struct ADPhantomCine
   {
   };

