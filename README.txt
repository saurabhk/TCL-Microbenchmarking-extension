I) Install PAPI (if necessary)
	1) Go to http://icl.cs.utk.edu/papi/
	2) Download the PAPI software
	3) tar xfz
	4) cd src
	5) ./configure
	6) make
	7) sudo make install
	8) sudo make install-man

II) check Perft makefile
	1) Check TCLI (path to tcl.h)
	2) Check LDFLAGS (path to tclstub, if not /usr/lib)
	3) Check TCLLIB (name of link option for Tcl stubs library)

III) make

IV) run
	1) make run (if you have rlwrap)
	2) else tclsh8.5
	3) load build/libperft.so
	4) Start instrumenting!
	
V) test
    make test	

You may need to modify LD_LIBRARY_PATH to include the location of libpapi.so

