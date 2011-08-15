#include<Perft_common.h>

struct _perfctr_event_info {
   char * name;                             // Event name
   char * description;                      // Event description (about one sentence)
   char * hdw_event_name;                   // Event name used in hardware documents
   char * um_em;                            // Unit Mask (P6) or Event Mask (P4) value
   int  event_id;                           // Event Id
   int  num_ctrs;                           // Number of counters where event can be counted
   int  ctrs[6];                            // Actual counter numbers
};


typedef struct _perfctr_event_info PERFCTR_EVENT_INFO;
const PERFCTR_EVENT_INFO *desc;


typedef int(*pGetPerfCounterEventNameList)(const char * const * * );
typedef int(*pGetPerfCounterEventIdFromName)(const char * );
typedef const PERFCTR_EVENT_INFO* (*pGetPerfCounterEventInfoFromId)(int);
typedef int (*pGetNumberOfPerfCounters)(void);


/*
 * Prototypes for procedures referenced only in this file:
 */
extern "C" int DLLEXPORT
ListEventsCmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);


extern "C" int DLLEXPORT 
HwInfoCmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);


 /*
 *______________________________________________
 *
 * ListEventsCmd --
 *
 * This function defines what to do if 'available_events'
 * command is called.
 * 
 * Results:
 * A list containing standard PAPI events with little description
 * of each is set as the result of the command.
 *________________________________________________
 */

extern "C" int DLLEXPORT 
ListEventsCmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    int retval;
    HINSTANCE hInstance;
    if(!(hInstance=LoadLibrary("perfutil.dll"))){
        cout << "could not load perfutil library" << endl;
        return 1;
    }

    pGetPerfCounterEventNameList GetEventList = (pGetPerfCounterEventNameList)GetProcAddress(hInstance, "GetPerfCounterEventNameList");
	if(!GetEventList)	{
		cerr<<"Error in loading GetPerfCounterEventNameList perfinsp API function";
		return 1;
	}
	pGetPerfCounterEventInfoFromId GetEventDesc=(pGetPerfCounterEventInfoFromId)GetProcAddress(hInstance, "GetPerfCounterEventInfoFromId");	
	if(!GetEventDesc)	{
		cerr<<"Error in loading GetPerfCounterEventInfoFromId perfinsp API function";
		return 1;
	}
	pGetPerfCounterEventIdFromName GetEventCodeFromName=(pGetPerfCounterEventIdFromName)GetProcAddress(hInstance, "GetPerfCounterEventIdFromName");	
	if(!GetEventCodeFromName)	{
		cerr<<"Error in loading GetPerfCounterEventIdFromName perfinsp API function";
		return 1;
	}

   
    char **data;
    AllocateStringArray(2*MAX_EVENTS,MAX_STRING_LENGTH,&data);
	
	char **temp;
    
	int eventsCount=GetEventList((const char * const * * )(&temp)) ;
	
	for(int i=eventsCount-1;i>=0;i--) {
		strcpy(data[2*i],temp[i]);
	}	
	
	for(int i=0;i<eventsCount;i++) {
		desc=GetEventDesc(GetEventCodeFromName((data)[2*i]));
		strcpy(data[2*i+1],desc->description);
	}
	
    char*eventList=Tcl_Merge(2*eventsCount, (const char *const *)data);
    DeallocateStringArray(2*MAX_EVENTS,&data);
    Tcl_SetResult(interp,eventList,TCL_DYNAMIC);
	return TCL_OK;
 }

 
/*
 *______________________________________________
 *
 * HwInfoCmd --
 *
 * This function returns the number of  hardware counters 
 * available on the machine
 *________________________________________________
 */
extern "C" int DLLEXPORT 
HwInfoCmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
	HINSTANCE hInstance;
    
    if(!(hInstance=LoadLibrary("perfutil.dll"))){
        cerr<< "could not load perfutil library"<<endl;
        return 1;
    }

	pGetNumberOfPerfCounters GetNoOfCounters=(pGetNumberOfPerfCounters)	GetProcAddress(hInstance, "GetNumberOfPerfCounters");
	if(!GetNoOfCounters)	{
		cerr<<"Error in loading GetNumberOfPerfCounters perfinsp API function";
		return 1;
	}	
	
	char result[40]="Number of Hardware Counters:";
    char counters[10];
    int noOfCounters=GetNoOfCounters();
    if(noOfCounters==0) {
        cerr<<"Error in getting information about hardware counters: \n";
        return 1;      
    }
    sprintf(counters,"%d",noOfCounters);
    strcat(result,counters);
 
    Tcl_SetObjResult(interp, Tcl_NewStringObj(result,-1));
    return TCL_OK;
}