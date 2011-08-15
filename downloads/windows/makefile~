### Tcl include dir
TCLI:=C:\Tcl\tcl8.5.10\generic

CXX:=C:\MinGW\bin\g++.exe

### Tcl lib dir
CXXFLAGS:=-I$(TCLI) -I.

### Tcl interpreter
TCLINTERP:=tclsh85

### Tcl libraries
TCLLIB:=C:\Tcl\tcl8.5.10\win\Release_VC9\tcl85.lib 

all: libperft

### link rule
libperft: list  selectevents main Perft_ensemble.cpp 
	$(CXX) -shared -o Perft.dll Perft_ensemble.cpp  $(CXXFLAGS) $(TCLLIB)  list.dll select.dll main.dll


main:	 Perft_common.h Perft_main.cpp
	$(CXX) -shared -o main.dll Perft_main.cpp  $(CXXFLAGS) $(TCLLIB)

selectevents: Perft_selectEvents.cpp
	$(CXX) -shared -o select.dll Perft_selectEvents.cpp $(CXXFLAGS) $(TCLLIB)

list:  Perft_listEvents.cpp
	$(CXX) -shared -o list.dll Perft_listEvents.cpp $(CXXFLAGS) $(TCLLIB)

test:
	$(TCLINTERP) ../test/Perft.test
	$(TCLINTERP) ../test/advanced/Cache.test
	$(TCLINTERP) ../test/advanced/Branch.test
	$(TCLINTERP) ../test/advanced/Sorting.test

