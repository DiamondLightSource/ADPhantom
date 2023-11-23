TOP=../..

include $(TOP)/configure/CONFIG

# -------------------------------
# Build a Support Module
# -------------------------------

LIBRARY_IOC += phantom
PROD += createParamIncludes

DBD += phantomCameraSupport.dbd

# The following are compiled and added to the support library
HDEPENDS = YES

createParamIncludes_SRCS += createParamIncludes.cpp
createParamIncludes_SRCS += phantomCameraSupport.cpp

phantom_SRCS += PhantomCamera.cpp
phantom_SRCS += phantomCameraSupport.cpp

phantom_LIBS += asyn
phantom_LIBS += ADBase
phantom_LIBS += busy

DATA += phantom_hdf5.xml

# We need to link against the EPICS Base libraries
phantom_LIBS += $(EPICS_BASE_IOC_LIBS)


include $(TOP)/configure/RULES
