Windows:
	I) Install Microsoft Visual Studio or MingW g++ (If necessary)

	II) Install perfinsp package (if necessary)
		1) Go to http://http://perfinsp.sourceforge.net/install_windows.html
		2) Download the perfinsp package (32-bit/64-bit as may be the case)
		3) cd /d c:\ibmperf 
		4) unzip {pi_win32.zip | pi_win64.zip}
		5) tinstall
	Each time you want to use perft extension, the path to 'perfutil.dll' should be present in your 'path' environment variable. The correct path can be set by running
	c:\ibmperf\bin\setrunenv in the same command prompt window.
	(This will set the runtime environment. This should be done in order to extension to locate 'perfutil.dll')

	III) choose one of the two makefiles (these correspond to MingW g++ and visual studio respectively)
		1) Check TCLI (path to tcl.h)
		2) Check TCLLIB (name of link option for Tcl stubs library [e.g. tcl85.lib in case you are using tcl8.5])
		3) Check CXX (path to MingW C++)
		4) Check TCLINTERP (tcl interpreter)

	IV) make (install make tool if not installed already)

	V) test
	    make test	

	VI) Put the build files at appropriate locations
		1) Create a directory Perft1.0 in your 'lib'/'library' directory inside the tcl installation directory [you may choose any other directory as per your convenience but you should change your auto_path variable accordingly.]
		2) Copy /windows/Perft.dll, /windows/pkgIndex.tcl and /lib/PerftWidget.tcl to this directory
	
	VII)  use the extension by including the perft package in your scripts
		e.g. you can add a line like
		package require Perft 1.0
	
