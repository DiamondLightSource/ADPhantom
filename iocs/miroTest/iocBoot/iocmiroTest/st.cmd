<envPaths
cd "$(TOP)"

epicsEnvSet "EPICS_TS_MIN_WEST", '0'


# Loading libraries
# -----------------

# Device initialisation
# ---------------------

dbLoadDatabase "dbd/phantomTest.dbd"
phantomTest_registerRecordDeviceDriver(pdbbase)

drvAsynIPPortConfigure("ctrlPort", "localhost:7115", 0, 0, 0)
drvAsynIPPortConfigure("dataPort", "localhost:7116", 0, 0, 0)

# phantomCameraConfig(portName, driverName, maxBuffers, maxMemory, priority, stackSize )
phantomCameraConfig("PHANTOM", "ctrlPort", "dataPort", 0, 0, 0, 0)

phantomCameraDebug("PHANTOM", "all", 1)

# Final ioc initialisation
# ------------------------

dbLoadRecords 'db/ADPhantom.db'
iocInit
