### Tcl include dir
TCLI:=C:\Tcl\tcl8.5.10\generic


### Tcl lib dir
CXXFLAGS:=/I$(TCLI) /I.

### Tcl interpreter
TCLINTREP:=tclsh85

### Tcl libraries
TCLLIB:=C:\Tcl\tcl8.5.10\win\Release_VC9\tcl85.lib 

all: libperft

### link rule
libperft: list  selectevents main Perft_ensemble.cpp 
	@cl /EHsc /MD /FePerft.dll Perft_ensemble.cpp  $(CXXFLAGS) /link $(TCLLIB)  list.lib select.lib main.lib /dll


main:	 Perft_common.h Perft_main.cpp
	cl /EHsc /MD /Femain.dll Perft_main.cpp  $(CXXFLAGS) /link $(TCLLIB) /dll

selectevents: Perft_selectEvents.cpp
	cl /EHsc /MD /Feselect.dll Perft_selectEvents.cpp $(CXXFLAGS) /link $(TCLLIB) /dll

list:  Perft_listEvents.cpp
	cl /EHsc /MD /Felist.dll Perft_listEvents.cpp $(CXXFLAGS)  /link $(TCLLIB) /dll

test:
	$(TCLINTREP) ./test/Perft.test
	$(TCLINTREP) ./test/advanced/Cache.test
	$(TCLINTREP) ./test/advanced/Branch.test
	$(TCLINTREP) ./test/advanced/Sorting.test

