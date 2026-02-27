TOP=../..

include $(TOP)/configure/CONFIG

# -------------------------------
# Build a Support Module
# -------------------------------

LIBRARY_IOC += phantom

DBD += ADPhantomSupport.dbd

# The following are compiled and added to the support library
HDEPENDS = YES

createParamIncludes_SRCS += ADPhantomSupport.cpp

phantom_SRCS += ADPhantom.cpp
phantom_SRCS += ADPhantomSupport.cpp

DATA += phantom_hdf5.xml

include $(ADCORE)/ADApp/commonLibraryMakefile


include $(TOP)/configure/RULES
