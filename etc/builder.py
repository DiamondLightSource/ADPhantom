from iocbuilder import Device, AutoSubstitution, Architecture, SetSimulation
from iocbuilder.arginfo import *
from iocbuilder.modules.asyn import Asyn, AsynPort, AsynIP

from iocbuilder.modules.ADCore import ADCore, ADBaseTemplate, NDFileTemplate, makeTemplateInstance, includesTemplates



class phantomCineTemplate(AutoSubstitution):
    TemplateFile="phantomCine.template"

class phantomFlashTemplate(AutoSubstitution):
    TemplateFile="phantomFlash.template"

@includesTemplates(ADBaseTemplate, NDFileTemplate)
class phantomCameraTemplate (AutoSubstitution):
    TemplateFile = 'phantomCamera.template'

class phantomLoadedFirst(Device):
    # Make sure our library gets included by dependent IOCs
    LibFileList = ['phantom']
    
    # This seems to be necessary
    AutoInstantiate = True

# Main class for the opto device
class ADPhantom(AsynPort):
    # We depend upon ADCore
    Dependencies = (phantomLoadedFirst, ADCore)

    # Make sure our DBD file gets used or created
    DbdFileList = ['ADPhantomSupport']
    UniqueName = "PORT"

    def __init__(self, PORT, P="P", R="R", ADDR=0, TIMEOUT=1000, CTRLPORT="CTRLPORT", DATAPORT="DATAPORT", MACADDRESS="MACADDRESS", CINE=0, **args):
    
        # Call init on Device superclass
        self.__super.__init__(PORT)

        
        # Store arguments away to use later
        self.PORT = PORT
        self.P = P
        self.R = R
        self.ADDR = ADDR
        self.TIMEOUT = TIMEOUT
        self.CTRLPORT = CTRLPORT
        self.DATAPORT = DATAPORT
        self.MACADDRESS = MACADDRESS
        self.CINE = CINE

        # Update the attributes of self from the commandline args
        self.__dict__.update(locals())

        # Make an instance of our template
        makeTemplateInstance(phantomCameraTemplate, locals(), args)
        # makeDb = phantomCameraTemplate(P=P, R=R, PORT=PORT, ADDR=ADDR, TIMEOUT=TIMEOUT)
        for i in range(1,64):
            makeDb = phantomCineTemplate(P=P, R=R, PORT=PORT, ADDR=ADDR, TIMEOUT=TIMEOUT, CINE=i)


    
    def InitialiseOnce(self):
        # Print some description of what we're doing in the startup script
        print "# Driver for ADPhantom"
        
    def Initialise(self):
        # Print the command to create the device in the startup script
        print "# Create driver"
        print "# ADPhantomConfig(const char *portName, const char *ctrlPort, const char *dataPort, const char * macAddress, int maxBuffers, size_t maxMemory, int priority,  int stackSize)"
        print "ADPhantomConfig(\"{0}\",\"{1}\",\"{2}\", \"{3}\",0, 0, 0,  0)".format(self.PORT, self.CTRLPORT, self.DATAPORT, self.MACADDRESS)
    
    # tell xmlbuilder what args to supply
    ArgInfo = ADBaseTemplate.ArgInfo + phantomCameraTemplate.ArgInfo + makeArgInfo(__init__,
        PORT    = Simple("Port name for the detector", str),
        P       = Simple("Device prefix", str),
        R       = Simple("Device suffix", str),
        ADDR    = Simple("Address of this controller", int),
        TIMEOUT = Simple("Timeout for communications, ms", int),
        CTRLPORT = Simple("Device Ports", str),
        DATAPORT = Simple("Device Ports", str),
        MACADDRESS = Simple("Server MAC address", str),
        CINE     = Simple("cine", int))
    
        

    
