ADPhantom
=======

An [EPICS](http://www.aps.anl.gov/epics) [areaDetector](http://cars.uchicago.edu/software/epics/areaDetector.html) driver for the Phantom T2410.

Credits
---------------------

Source code developed by [Observatory Sciences Ltd.](https://www.observatorysciences.co.uk) for [Diamond Light Source Ltd](https://www.diamond.ac.uk). Development builds on [miroCamera](https://github.com/dls-controls/miroCamera) driver.

Supported platforms
-------------------

This module is designed for the Phantom T2410 camera. Some compatability with other Phantom models may be possible but this has not been tested.
Developed and tested on RHEL7 using EPICS BASE R3.14.12.7

10 Gb downloads
-------------------

This driver uses the [libpcap library](https://www.tcpdump.org/) to allow downloads over the camera's 10 Gb connection. This requires the driver to either be run using sudo or to be given the CAP_NET_RAW and CAP_NET_ADMIN kernel permissions.

The 10 Gb downloads use an 'IEEE802a OUI Extended Ethertype' protocol to transmit their datapackets rather than TCP or UDP. Our interpretation of this protocol is that in addition to 19 bytes of ethernet header the first 13 bytes of 'data' form an additional header. The remaining data is image data. The 18th byte of the ethernet header appears to be a counter iterating by one for each packet. Very occasionally this counter will reset to zero at the start of a new frame. This counter allows any missed packets to be detected. The final packet of a frame is smaller than the rest allowing additional verification that packets have not been missed.

The other bytes in the ethernet header and additional header are not used in the driver, however the last four bytes of the additonal header are constant and correspond to the total number of bytes in each frame.
