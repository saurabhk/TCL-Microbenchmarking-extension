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
libperft: papif Perft_common.h list others init runfile runscript selectevents general Perft_ensemble.cpp
	@g++ ./build/papif ./build/list ./build/others ./build/init ./build/runfile ./build/runscript ./build/selectevents ./build/general -shared -o ./build/libperft.so $(CXXFLAGS) Perft_ensemble.cpp $(LDFLAGS) $(TCLLIB) -lpapi

run: libperft
	@rlwrap /usr/bin/ActiveTcl-8.5/bin/tclsh
	
runscript:	 Perft_common.h papif others Perft_runScript.cpp
	@g++ $(CXXFLAGS) ./build/papif ./build/others -shared -o ./build/runscript Perft_runScript.cpp

runfile:	 Perft_common.h papif others Perft_runFile.cpp
	@g++ $(CXXFLAGS) ./build/papif ./build/others -shared -o ./build/runfile Perft_runFile.cpp

selectevents:	Perft_common.h papif others Perft_selectEvents.cpp
	@g++ $(CXXFLAGS) ./build/papif ./build/others -shared -o ./build/selectevents Perft_selectEvents.cpp

list: Perft_common.h papif others Perft_listEvents.cpp
	@g++ $(CXXFLAGS) ./build/papif ./build/others -shared -o ./build/list Perft_listEvents.cpp

init: Perft_common.h papif others Perft_init.cpp
	@g++ $(CXXFLAGS) ./build/papif ./build/others -shared -o ./build/init Perft_init.cpp
	
general: Perft_common.h Perft_general.cpp
	@g++ $(CXXFLAGS) ./build/papif ./build/others -shared -o ./build/general Perft_general.cpp

papif: Perft_common.h  Perft_papi.cpp others
	@g++ $(CXXFLAGS) -shared -o  ./build/papif ./build/others -lpapi Perft_papi.cpp

others: Perft_common.h 
	@g++ $(CXXFLAGS) -shared -o  ./build/others -lpapi Perft_others.cpp

test: libperft
	@tclsh8.5 ./test/Perft.test

