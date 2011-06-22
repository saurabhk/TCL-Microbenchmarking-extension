#ifndef _PERF
#define _PERF


#include<stdlib.h>
#include <tcl.h>
#include <papi.h>
#include<string.h>
#include<stdio.h>
#include<stdint.h>

#define PAPI_MAX_EVENTS 200
#define PAPI_MAX_STRING_LENGTH 500

struct papiState_t {
    int eventSet;
    int numberOfEvents;
    int64_t counterValues[PAPI_MAX_EVENTS];
    char**eventsData;
} 
papiState={0};


/*
 * Perft_init.cpp
 */


int 
InitCmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);

/*
 * Perft_listEvents.cpp
 */
 
int 
ListEventsCmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);


/*
 * Perft_selectEvents.cpp
 */


int 
SelectEventsCmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);



/*
 * Perft_runFile.cpp
 */

int 
RunFileCmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);

/*
 * Perft_runScript.cpp
 */


int 
RunScriptCmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);

/*
 * Perft_general.cpp
 */

int 
HwInfoCmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);

/*
 * Perft_papi.cpp
 */
 
void
HandlePapiError(int errcode);

void
InitPapi() ;

void
ResetEventSet();

void
InitPapiMultiplexing();

void
StartCounting();

void
StopCounting();

void
AccumCounting();

/*
 * Perft_others.cpp
 */
 
char** 
AllocateStringArray(int noOfStrings,int size);

void
DeallocateStringArray(int noOfStrings,char**memPtr);

#endif
