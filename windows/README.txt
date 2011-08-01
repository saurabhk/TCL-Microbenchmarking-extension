I) Install Microsoft Visual Studio (If necessary) or otherwise MingW g++ can be used 
   There are two makefiles in this package for windows one corresoponding to MSVC and other one for MingW g++. You should use one out of them as may be the case.
   
II) Install perfinsp package (if necessary)
	1) Go to http://http://perfinsp.sourceforge.net/install_windows.html
	2) Download the perfinsp package (32-bit/64-bit as may be the case)
	3) cd /d c:\ibmperf 
	4) unzip {pi_win32.zip | pi_win64.zip}
	5) tinstall
	6)  cd /d c:\ibmperf\bin 
		setrunenv 
		(This will set the runtime environment. This should be done in order to extension to locate 'perfutil.dll')

III) check Perft makefile ( in 'windows' subfolder)
	1) Check TCLI (path to tcl.h)
	2) Check CXX (path to MingW g++ if you intend to use it)
	3) Check TCLLIB (name of link option for Tcl stubs library)
  4) Check TCLINTREP (path to tclsh interpreter) 

IV) make (install make tool if not installed already)

V) run
	1) tclsh8.5
	2) load ./Perft.dll
	3) Start instrumenting!
	
VI) test
    make test	


