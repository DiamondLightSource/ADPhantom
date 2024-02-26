#!../../bin/linux-x86_64/phantomApp

errlogInit(20000)

epicsEnvSet("EPICS_CA_MAX_ARRAY_BYTES",  64000000)
epicsEnvSet("EPICS_CA_AUTO_ADDR_LIST",   "NO")
epicsEnvSet("EPICS_CA_ADDR_LIST",        "127.0.0.255")
epicsEnvSet("EPICS_CAS_INTF_ADDR_LIST",  "127.0.0.1")

< envPaths
dbLoadDatabase("$(TOP)/dbd/phantomApp.dbd")
phantomApp_registerRecordDeviceDriver(pdbbase) 

# Prefix for all records
epicsEnvSet("PREFIX", "BL12:IOC30:")
# The port name for the detector
epicsEnvSet("PORT",   "PHANTOM-T2410")
# The queue size for all plugins
epicsEnvSet("QSIZE",  "20")
# The maximim image width; used for row profiles in the NDPluginStats plugin
epicsEnvSet("XSIZE",  "1280")
# The maximim image height; used for column profiles in the NDPluginStats plugin
epicsEnvSet("YSIZE",  "960")
# The maximum number of time seried points in the NDPluginStats plugin
epicsEnvSet("NCHANS", "2048")
# The maximum number of frames buffered in the NDPluginCircularBuff plugin
epicsEnvSet("CBUFFS", "500")
# The search path for database files
epicsEnvSet("EPICS_DB_INCLUDE_PATH", "$(AREADETECTOR)/db:$(ADPHANTOM)/db")
# Size of data allowed 
epicsEnvSet("EPICS_CA_MAX_ARRAY_BYTES", 20000000)


# Configure control and data TCP/IP socket connections. Default ports should be 7115 and 7116 respectively.
drvAsynIPPortConfigure("ctrlPort", "172.23.112.23:7115", 0, 0, 0)
drvAsynIPPortConfigure("dataPort", "172.23.112.23:7116", 0, 0, 0)

# ADPhantomConfig(const char *portName, const char *ctrlPort, const char *dataPort, const char * macAddress, const char * interface, int maxBuffers, size_t maxMemory, int priority,  int stackSize)
ADPhantomConfig("phantom.cam","ctrlPort","dataPort", "1423f21fdeb0", "p1p1", 0, 0, 0,  0)
# Enable debugging for certain functions
#ADPhantomDebug("$(PORT)", "readoutDataStream", 1);

asynSetTraceIOMask($(PORT), 0, 2)
#asynSetTraceMask($(PORT), 0, 0xff)

# Load base camera records
dbLoadRecords("$(ADPHANTOM)/db/phantomCamera.template", "P=$(PREFIX),R=cam1:,PORT=$(PORT),ADDR=0,TIMEOUT=1")

# Load a template for each configured cine
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=1, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=2, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=3, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=4, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=5, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=6, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=7, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=8, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=9, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=10, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=11, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=12, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=13, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=14, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=15, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=16, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=17, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=18, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=19, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=20, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=21, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=22, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=23, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=24, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=25, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=26, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=27, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=28, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=29, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=30, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=31, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=32, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=33, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=34, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=35, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=36, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=37, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=38, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=39, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=40, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=41, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=42, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=43, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=44, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=45, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=46, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=47, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=48, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=49, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=50, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=51, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=52, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=53, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=54, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=55, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=56, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=57, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=58, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=59, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=60, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=61, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=62, PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(ADPHANTOM)/db/phantomCine.template", "P=$(PREFIX),R=cam1:, CINE=63, PORT=$(PORT),ADDR=0,TIMEOUT=1")

#
# Create a standard arrays plugin, set it to get data from Driver.
# int NDStdArraysConfigure(const char *portName, int queueSize, int blockingCallbacks, const char *NDArrayPort, int NDArrayAddr, int maxBuffers, size_t maxMemory,
#                          int priority, int stackSize, int maxThreads)
NDStdArraysConfigure("Image1", 3, 0, "$(PORT)", 0)
dbLoadRecords("$(ADCORE)/db/NDStdArrays.template", "P=$(PREFIX),R=NDSA:,PORT="Image1",ADDR=0,NDARRAY_PORT=$(PORT),TIMEOUT=1,TYPE=Int16,FTVL=SHORT,NELEMENTS=6000000")

# Load all other plugins using commonPlugins.cmd
#< $(ADCORE)/iocBoot/commonPlugins.cmd

# Create an HDF5 file saving plugin
NDFileHDF5Configure("FileHDF1", $(QSIZE), 0, "$(PORT)", 0)
dbLoadRecords("NDFileHDF5.template",  "P=$(PREFIX),R=HDF1:,PORT=FileHDF1,ADDR=0,TIMEOUT=1,XMLSIZE=2048,NDARRAY_PORT=$(PORT)")

#asynSetTraceMask($(PORT),0,0x09)
#asynSetTraceMask($(PORT),0,0x11)
iocInit()
