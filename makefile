### Tcl include dir
TCLI:=/usr/include/tcl8.5
#TCLI:=/usr/bin/ActiveTcl-8.5/include

### Tcl lib dir

CXXFLAGS:=-I$(TCLI) -I./

### needed for 64 bit libraries
CXXFLAGS+=-fPIC

LDFLAGS:=-L/usr/bin/ActiveTcl-8.5/lib
TCLLIB:=-ltclstub8.5

all: libperft

### link rule
libperft: papif Perft_common.h list others init runfile runscript selectevents
	@g++ ./build/papif ./build/list ./build/others ./build/init ./build/runfile ./build/runscript ./build/selectevents -shared -o ./build/libperft.so $(CXXFLAGS) Perft_ensemble.cpp $(LDFLAGS) $(TCLLIB) -lpapi

run: libperft
	@rlwrap /usr/bin/ActiveTcl-8.5/bin/tclsh
	
runscript:	 Perft_common.h papif others
	@g++ $(CXXFLAGS) ./build/papif ./build/others -shared -o ./build/runscript Perft_runScript.cpp

runfile:	 Perft_common.h papif others
	@g++ $(CXXFLAGS) ./build/papif ./build/others -shared -o ./build/runfile Perft_runFile.cpp

selectevents:	Perft_common.h papif others
	@g++ $(CXXFLAGS) ./build/papif ./build/others -shared -o ./build/selectevents Perft_selectEvents.cpp

list: Perft_common.h papif others
	@g++ $(CXXFLAGS) ./build/papif ./build/others -shared -o ./build/list Perft_listEvents.cpp

init: Perft_common.h papif others
	@g++ $(CXXFLAGS) ./build/papif ./build/others -shared -o ./build/init Perft_init.cpp

papif: Perft_common.h others
	@g++ $(CXXFLAGS) -shared -o  ./build/papif ./build/others -lpapi Perft_papi.cpp

others: Perft_common.h 
	@g++ $(CXXFLAGS) -shared -o  ./build/others -lpapi Perft_others.cpp

