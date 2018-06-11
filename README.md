ROOT wrappers for nuphase data classes 
---

Requires ROOT (only tested with ROOT 6). If you want to read in raw data, that hasn't already been rootified, you also need libnuphase (see below). 

In most cases, you just need to: 

 - set NUPHASE_INSTALL_DIR to something sensible  (e.g., in bash,  export NUPHASE_INSTALL_DIR=~/nuphase) 
 - make
 - make install  

 This will install the nuphaseroot libraries into that directory. It is then
 probably a good idea to add $NUPHASE_INSTALL_DIR/lib to your LD_LIBRARY_PATH
 (or DYLD_LIBRARY_PATH on a mac, although never tested on a mac). 


There is an example of how to use python to read in rootified data under examples. 


---

If you need to convert raw data, you should configure (make configure or
ccmake) and enable LIBNUPHASE_SUPPORT. If libnuphase is in NUPHASE_INSTALL_DIR
you can leave LIBNUPHASE_PATH empty, otherwise you should set it properly. 








