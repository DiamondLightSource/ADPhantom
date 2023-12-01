TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

#=============================
# Build the IOC application

PROD_IOC = phantomTest
# phantomTest.dbd will be created and installed
DBD += phantomTest.dbd

# phantomTest.dbd will be made up from these files:
phantomTest_DBD += base.dbd
phantomTest_DBD += phantomCameraSupport.dbd
phantomTest_DBD += asyn.dbd
phantomTest_DBD += drvAsynIPPort.dbd
phantomTest_DBD += ADSupport.dbd 
phantomTest_DBD += busySupport.dbd

# phantomTest_registerRecordDeviceDriver.cpp derives from phantomTest.dbd
phantomTest_SRCS += phantomTest_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
phantomTest_SRCS_DEFAULT += phantomTestMain.cpp
phantomTest_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#phantomTest_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Link in the support libraries
phantomTest_LIBS += phantom
phantomTest_LIBS += asyn
phantomTest_LIBS += ADBase
phantomTest_LIBS += busy

# Finally link to the EPICS Base libraries
phantomTest_LIBS += $(EPICS_BASE_IOC_LIBS)

#=============================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

