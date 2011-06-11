
libperft: papif Perft_common.h list others init runfile runscript selectevents
	g++ ./build/papif ./build/list ./build/others ./build/init  ./build/runfile ./build/runscript ./build/selectevents -shared -o ./build/libperft.so -I/usr/bin/ActiveTcl-8.5/include -I./ Perft_ensemble.cpp -L/usr/bin/ActiveTcl-8.5/lib  -ltclstub8.5 -lpapi  
	rlwrap /usr/bin/ActiveTcl-8.5/bin/tclsh
	
runscript:	 Perft_common.h papif others
	g++ ./build/papif ./build/others -shared -o  ./build/runscript -I/usr/bin/ActiveTcl-8.5/include -I./ Perft_runScript.cpp
runfile:	 Perft_common.h papif others
	g++ ./build/papif ./build/others -shared -o  ./build/runfile -I/usr/bin/ActiveTcl-8.5/include -I./ Perft_runFile.cpp
selectevents:	Perft_common.h papif others
	g++ ./build/papif ./build/others -shared -o  ./build/selectevents -I/usr/bin/ActiveTcl-8.5/include -I./ Perft_selectEvents.cpp
list: Perft_common.h papif others
	g++ ./build/papif ./build/others -shared -o  ./build/list -I/usr/bin/ActiveTcl-8.5/include -I./ Perft_listEvents.cpp
init: Perft_common.h papif others
	g++ ./build/papif ./build/others -shared -o  ./build/init -I/usr/bin/ActiveTcl-8.5/include -I./ Perft_init.cpp
papif: Perft_common.h others
	g++ -shared -o  ./build/papif ./build/others -I/usr/bin/ActiveTcl-8.5/include -I./ -lpapi Perft_papi.cpp
others: Perft_common.h 
	g++ -shared -o  ./build/others -I/usr/bin/ActiveTcl-8.5/include -I./ -lpapi Perft_others.cpp
