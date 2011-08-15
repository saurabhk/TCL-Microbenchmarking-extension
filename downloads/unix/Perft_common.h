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


/*
 * Perft_main.cpp
 */

void
HandlePapiError(int errcode);

int
InitPapi() ;

int
ResetEventSet();

int
InitPapiMultiplexing();

int
StartCounting();

int
StopCounting();

int
AccumCounting();

int 
InitCmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);


int 
RunFileCmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);


int 
RunScriptCmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);


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
 * Perft_general.cpp
 */

int 
HwInfoCmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);


/*
 * Perft_others.cpp
 */
 
void 
AllocateStringArray(int noOfStrings,int size,char***memPtr);

void
DeallocateStringArray(int noOfStrings,char***memPtr);

#endif
