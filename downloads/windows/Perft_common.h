#ifndef _PERF
#define _PERF

#include<tcl.h>
#include <iostream>
#include <windows.h>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>

using namespace std;

#define PACKAGE_VERSION "1.1"
#define PACKAGE_NAME "Perft"
#define MAX_EVENTS 200
#define MAX_STRING_LENGTH 3000


/*
 * Structure to store the current state : list and count of current events etc.
 */
struct perfState_t {
	int eventSet[MAX_EVENTS];
    int numberOfEvents;
    long long counterValues[MAX_EVENTS];
    char**eventsData;
} 
perfState={0};



/*
 * Perft_listEvents.cpp
 */
 
extern "C" int DLLEXPORT 
ListEventsCmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);

extern "C" int DLLEXPORT 
HwInfoCmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);




/*
 * Perft_selectEvents.cpp
 */

 extern "C" int DLLEXPORT 
SelectEventsCmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);





/*
 * Perft_main.cpp
 */

extern "C" int DLLEXPORT 
InitCmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);

extern "C" int DLLEXPORT 
RunScriptCmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);

extern "C" int DLLEXPORT 
RunFileCmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);





/*
 * Perft_others.cpp
 */

extern "C" void  DLLEXPORT
AllocateStringArray(int noOfStrings,int size);

extern "C" void  DLLEXPORT
DeallocateStringArray(int noOfStrings,char**memPtr);

 /*
 *______________________________________________
 *
 * AllocateStringArray --
 *
 * This function allocates dynamic memory for 
 * an array of strings where 'size' is the size of each string
 * 
 *
 * Results:
 * Dynamic memory allocation for string array
 *________________________________________________
 */
void 
AllocateStringArray(int noOfStrings,int size,char*** memPtr)
{
    *memPtr = (char**)malloc(noOfStrings * sizeof(char*));
    for (int  i = 0; i < noOfStrings; i++) {
        (*memPtr)[i] =(char *) malloc(size* sizeof(char));
   }
}

 /*
 *______________________________________________
 *
 * DeallocateStringArray --
 *
 * This function deallocates dynamic memory of 
 * an array of strings where 'noOfStrings' is the no. of strings
 * 
 *
 * Results:
 * Dynamic memory deallocation for string array
 *________________________________________________
 */
void
DeallocateStringArray(int noOfStrings,char***memPtr)
{
    for (int  i = 0; i < noOfStrings; i++) {
        free((*memPtr)[i]);
     }
     free(*memPtr);
}

#endif
