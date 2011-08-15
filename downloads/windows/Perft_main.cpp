#include<Perft_common.h>

/*
 * For temporarily storing counter results to estimate surplus results
 */
long long tempCounterValues[MAX_EVENTS]; 

/*
 * Type definitions for Functions imported from Perfinsp C API
 */
typedef int(*pResumePerfCounterEvent)(int);
typedef int(*pIsPerfCounterEventActive)(int);
typedef int (*pTerminatePerfCounterEvent)(int);
typedef int(*pGetPerfCounterEventIdFromName)(const char * );
typedef int (*pAnyPerfCounterEventActive)(void);
typedef int(*pInitPerfCounterEvent)(int , int );
typedef int(*pResetPerfCounterEvent)(int);
typedef int(*pStopPerfCounterEvent)(int);
typedef int(*pReadPerfCounterForEvent)(int , UINT64 *, UINT64 * );
typedef int (*pGetActiveProcessorCount)(void);
typedef const char *(*pGetPerfCounterEventNameFromId)(int);
typedef int(*pGetNumberOfSimultaneousPerfCounterEventsSupported)(void);


/*
 * Prototypes for procedures referenced only in this file:
 */
extern "C" int DLLEXPORT
InitCmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);


extern "C" int DLLEXPORT
RunScriptCmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);


extern "C" int DLLEXPORT
RunFileCmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);


 /*
 *______________________________________________
 *
 *InitCmd --
 *
 * This function is to add events to papi event set.
 * A list of events is taken as argument.
 * 
 * Results:
 * The eventset is initialized.
 *________________________________________________
 */
int 
InitCmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{

	if(objc!=2) {
        cerr<<"Usage: perft::init listOfEvents";
        return 1;
    }

    HINSTANCE hInstance;
    
    if(!(hInstance=LoadLibrary("perfutil.dll"))){
        cerr << "could not load perfutil library" << endl;
        return 1;
    }


	pTerminatePerfCounterEvent TerminateEvent=(pTerminatePerfCounterEvent)GetProcAddress(hInstance, "TerminatePerfCounterEvent");
	if(!TerminateEvent)	{
		cerr<<"Error in loading TerminatePerfCounterEvent perfinsp API function";
		return 1;
	}
	pGetPerfCounterEventIdFromName EventNameToCode=(pGetPerfCounterEventIdFromName)GetProcAddress(hInstance, "GetPerfCounterEventIdFromName");			
	if(!EventNameToCode)	{
		cerr<<"Error in loading GetPerfCounterEventIdFromName perfinsp API function";
		return 1;
	}
	pAnyPerfCounterEventActive IsAnyEventActive=(pAnyPerfCounterEventActive)GetProcAddress(hInstance, "AnyPerfCounterEventActive");			
	if(!IsAnyEventActive)	{
		cerr<<"Error in loading AnyPerfCounterEventActive perfinsp API function";
		return 1;
	}
	pGetNumberOfSimultaneousPerfCounterEventsSupported MaxSimEvents=(pGetNumberOfSimultaneousPerfCounterEventsSupported)GetProcAddress(hInstance, "GetNumberOfSimultaneousPerfCounterEventsSupported");				
	if(!MaxSimEvents)	{
		cerr<<"Error in loading GetNumberOfSimultaneousPerfCounterEventsSupported perfinsp API function";
		return 1;
	}
	
    int retval,eventCode;
    char* arg=Tcl_GetString(objv[1]);
    int argc, eventList;
    const char **argv;
    eventList= Tcl_SplitList(interp, arg, &argc,  &argv);
    char **index=(char **)argv;  
    
    if(perfState.eventsData==NULL) {
        AllocateStringArray(MAX_EVENTS,MAX_STRING_LENGTH,&perfState.eventsData);
    } 
    // To terminate all events
	if(IsAnyEventActive()) {
		retval=TerminateEvent(-1); 
		if (retval) {
			cerr<<"Error terminating already active events"<<endl;
			return 1;
		}
	}	
     
      
    perfState.numberOfEvents=0;
    
	if(argc>MaxSimEvents()) {
		cerr<<"Maximum no. of simultaneous events supported: "<<MaxSimEvents()<<endl;
		return 1;
	}
	
       
    while(perfState.numberOfEvents<argc) {
        eventCode=EventNameToCode(*index);
        if (!eventCode) {
            cerr<<"Error in getting information about event name: "<<*index<<endl;
            return 1;
        }
        perfState.eventsData[perfState.numberOfEvents]=*index;
		perfState.eventSet[perfState.numberOfEvents]=eventCode;
        index+=1;
        perfState.numberOfEvents+=1;
    }

    Tcl_SetObjResult(interp, Tcl_NewStringObj("Hardware counters initialized", -1));
    return TCL_OK;        
}    



 /*
 *______________________________________________
 *
 * RunScriptCmd --
 *
 * This function is to investigate the run time 
 * behaviour of a tcl script passes as command line argument
 * Before calling this function, InitCommand() must be called. 
 * Results:
 * The recorings of hardware counters is returned.
 *________________________________________________
 */
int 
RunScriptCmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{

 if(objc<2) {
        fprintf(stderr,"Usage: perft::run_script script");
        return 1;
    }        

	int doAccumulate=0;
	UINT64  ctr_value[10];
	UINT64  cyc_value[10];
	char* arg=Tcl_GetString(objv[1]);
    int counter=0;
	int retval;
	int noOfProcessors;
    Tcl_Obj * result;
    Tcl_Obj* event_name;
    Tcl_Obj* counter_value;
	HINSTANCE hInstance;
    
    if(!(hInstance=LoadLibrary("perfutil.dll"))){
        cerr << "could not load perfutil library" << endl;
        return 1;
    }	
	
	
	
    if(objc>=3) {
        char* flag=Tcl_GetString(objv[2]);        
        if(!strcmp("-a",flag))
            doAccumulate=1;
    }
    
    if(perfState.numberOfEvents<=0){
        cerr<<"First use perft::init/perft::select_events to set the eventset\n";
        return 1;
    }        
            
    
	pInitPerfCounterEvent InitEvent=(pInitPerfCounterEvent)GetProcAddress(hInstance, "InitPerfCounterEvent");
	if(!InitEvent)	{
		cerr<<"Error in loading InitPerfCounterEvent perfinsp API function";
		return 1;
	}
	pReadPerfCounterForEvent ReadCounter=(pReadPerfCounterForEvent) GetProcAddress(hInstance, "ReadPerfCounterEvent");
	if(!ReadCounter)	{
		cerr<<"Error in loading ReadPerfCounterEvent perfinsp API function";
		return 1;
	}
	pResetPerfCounterEvent ResetCounter=(pResetPerfCounterEvent) GetProcAddress(hInstance, "ResetPerfCounterEvent");
	if(!ResetCounter)	{
		cerr<<"Error in loading ResetPerfCounterEvent perfinsp API function";
		return 1;
	}
	pStopPerfCounterEvent StopCounter=(pStopPerfCounterEvent) GetProcAddress(hInstance, "StopPerfCounterEvent");
	if(!StopCounter)	{
		cerr<<"Error in loading StopPerfCounterEvent perfinsp API function";
		return 1;
	}
    pIsPerfCounterEventActive IsEventActive=(pIsPerfCounterEventActive)GetProcAddress(hInstance, "IsPerfCounterEventActive");			
	if(!IsEventActive)	{
		cerr<<"Error in loading IsPerfCounterEventActive perfinsp API function";
		return 1;
	}
	pResumePerfCounterEvent ResumeEvent=(pResumePerfCounterEvent)GetProcAddress(hInstance, "ResumePerfCounterEvent");
	if(!ResumeEvent)	{
		cerr<<"Error in loading ResumePerfCounterEvent perfinsp API function";
		return 1;
	}
	pGetActiveProcessorCount ProcessorCount=(pGetActiveProcessorCount)GetProcAddress(hInstance, "PiGetActiveProcessorCount");
	if(!ProcessorCount)	{
		cerr<<"Error in loading GetActiveProcessorCount perfinsp API function";
		return 1;
	}	
	pGetPerfCounterEventNameFromId EventNameFromId=(pGetPerfCounterEventNameFromId)GetProcAddress(hInstance, "GetPerfCounterEventNameFromId");	
    if(!EventNameFromId)	{
		cerr<<"Error in loading GetPerfCounterEventNameFromId perfinsp API function";
		return 1;
	}

	
	noOfProcessors=ProcessorCount();

// Estimate surplus results here
for (int iter=1;iter<5;iter++)
{
	for(int i=0;i<perfState.numberOfEvents;i++) {
		if(!IsEventActive(perfState.eventSet[i])) {
			retval=InitEvent(perfState.eventSet[i],2);
			if(retval) {
				cerr<<"Error initialising the event "<<perfState.eventSet[i]<<" "<<EventNameFromId(perfState.eventSet[i]);
				return 1;
			}
		} else {
			ResumeEvent(perfState.eventSet[i]);
		}
	tempCounterValues[i]=0;	
    }
	
    if(doAccumulate==0) {
        retval=ResetCounter(-1);
        if(retval) {
			cerr<<"Error resetting the event counters"<<endl;
            return 1;
        }
    }        
	
	// do nothing here
	
	for(int i=0;i<perfState.numberOfEvents;i++) {
		retval=ReadCounter(perfState.eventSet[i],ctr_value,cyc_value);
	    if(retval) {
			cerr<<"Error reading the event counters\n";
			return 1;
		}
		for(int j=0;j<noOfProcessors;j++) {
			tempCounterValues[i]+=ctr_value[j];
		}	
	}
	
 	retval=StopCounter(-1);
    if(retval) {
        cerr<<"Error stopping the event counters\n";
        return 1;
    }


// Do the Actual Counting here
	
	for(int i=0;i<perfState.numberOfEvents;i++) {
		if(!IsEventActive(perfState.eventSet[i])) {
			retval=InitEvent(perfState.eventSet[i],2);
			if(retval) {
				cerr<<"Error initialising the event "<<perfState.eventSet[i]<<" "<<EventNameFromId(perfState.eventSet[i]);
				return 1;
			}
		} else {
			ResumeEvent(perfState.eventSet[i]);
		}
	perfState.counterValues[i]=0;	
    }
	
    if(doAccumulate==0) {
        retval=ResetCounter(-1);
        if(retval) {
			cerr<<"Error resetting the event counters"<<endl;
            return 1;
        }
    }        
	}
// run the script here
    retval=Tcl_EvalObjEx(interp, objv[1], TCL_EVAL_GLOBAL);
    if(retval) {
       cerr<<"Error running the script\n";
       return 1;
    }

	
	for(int i=0;i<perfState.numberOfEvents;i++) {
		retval=ReadCounter(perfState.eventSet[i],ctr_value,cyc_value);
	    if(retval) {
			cerr<<"Error reading the event counters\n";
			return 1;
		}
		for(int j=0;j<noOfProcessors;j++) {
			perfState.counterValues[i]+=ctr_value[j];
		}	
	}

	retval=StopCounter(-1);
    if(retval) {
        cerr<<"Error stopping the event counters\n";
        return 1;
    }


	
    result=Tcl_NewListObj(0,NULL);
    
    while(counter<perfState.numberOfEvents) {
        event_name=Tcl_NewStringObj((const char *)perfState.eventsData[counter],-1);
        retval=Tcl_ListObjAppendElement(interp, result,event_name);
        if(retval) {
            cerr<<"Error adding event name to tcl list obj\n";
            return 1;
        }
        counter_value=Tcl_NewWideIntObj(perfState.counterValues[counter]-tempCounterValues[counter]);
        retval=Tcl_ListObjAppendElement(interp, result,counter_value);
        if(retval) {
            cerr<<"Error adding counter value to tcl list obj\n";
            return 1;
        }        
        counter++;
	}


	
	
    Tcl_SetObjResult(interp,result);
    return TCL_OK;  
  
} 

 /*
 *______________________________________________
 *
 * RunFileCmd --
 *
 * This function is to investigate the run time 
 * behaviour of a tcl program written in a file.
 * Before calling this function, InitCommand() must be called. 
 * Results:
 * The recorings of hardware counters is returned.
 *________________________________________________
 */
int 
RunFileCmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{

	if(objc<2) {
        fprintf(stderr,"Usage: perft::run_script script");
        return 1;
    }        

	int doAccumulate=0;
	UINT64  ctr_value[10];
	UINT64  cyc_value[10];
	char* arg=Tcl_GetString(objv[1]);
    int counter=0;
	int retval;
	int noOfProcessors;
    Tcl_Obj * result;
    Tcl_Obj* event_name;
    Tcl_Obj* counter_value;
	HINSTANCE hInstance;
    
    if(!(hInstance=LoadLibrary("perfutil.dll"))){
        cerr << "could not load perfutil library" << endl;
        return 1;
    }	
	
	
	
    if(objc>=3) {
        char* flag=Tcl_GetString(objv[2]);        
        if(!strcmp("-a",flag))
            doAccumulate=1;
    }
    
    if(perfState.numberOfEvents<=0){
        cerr<<"First use perft::init/perft::select_events to set the eventset\n";
        return 1;
    }        
            
    
	pInitPerfCounterEvent InitEvent=(pInitPerfCounterEvent)GetProcAddress(hInstance, "InitPerfCounterEvent");
	if(!InitEvent)	{
		cerr<<"Error in loading InitPerfCounterEvent perfinsp API function";
		return 1;
	}
	pReadPerfCounterForEvent ReadCounter=(pReadPerfCounterForEvent) GetProcAddress(hInstance, "ReadPerfCounterEvent");
	if(!ReadCounter)	{
		cerr<<"Error in loading ReadPerfCounterEvent perfinsp API function";
		return 1;
	}
	pResetPerfCounterEvent ResetCounter=(pResetPerfCounterEvent) GetProcAddress(hInstance, "ResetPerfCounterEvent");
	if(!ResetCounter)	{
		cerr<<"Error in loading ResetPerfCounterEvent perfinsp API function";
		return 1;
	}
	pStopPerfCounterEvent StopCounter=(pStopPerfCounterEvent) GetProcAddress(hInstance, "StopPerfCounterEvent");
	if(!StopCounter)	{
		cerr<<"Error in loading StopPerfCounterEvent perfinsp API function";
		return 1;
	}
    pIsPerfCounterEventActive IsEventActive=(pIsPerfCounterEventActive)GetProcAddress(hInstance, "IsPerfCounterEventActive");			
	if(!IsEventActive)	{
		cerr<<"Error in loading IsPerfCounterEventActive perfinsp API function";
		return 1;
	}
	pResumePerfCounterEvent ResumeEvent=(pResumePerfCounterEvent)GetProcAddress(hInstance, "ResumePerfCounterEvent");
	if(!ResumeEvent)	{
		cerr<<"Error in loading ResumePerfCounterEvent perfinsp API function";
		return 1;
	}
	pGetActiveProcessorCount ProcessorCount=(pGetActiveProcessorCount)GetProcAddress(hInstance, "PiGetActiveProcessorCount");
	if(!ProcessorCount)	{
		cerr<<"Error in loading GetActiveProcessorCount perfinsp API function";
		return 1;
	}	
	pGetPerfCounterEventNameFromId EventNameFromId=(pGetPerfCounterEventNameFromId)GetProcAddress(hInstance, "GetPerfCounterEventNameFromId");	
    if(!EventNameFromId)	{
		cerr<<"Error in loading GetPerfCounterEventNameFromId perfinsp API function";
		return 1;
	}

	
	noOfProcessors=ProcessorCount();

// Estimate surplus results here

	for(int i=0;i<perfState.numberOfEvents;i++) {
		if(!IsEventActive(perfState.eventSet[i])) {
			retval=InitEvent(perfState.eventSet[i],2);
			if(retval) {
				cerr<<"Error initialising the event "<<perfState.eventSet[i]<<" "<<EventNameFromId(perfState.eventSet[i]);
				return 1;
			}
		} else {
			ResumeEvent(perfState.eventSet[i]);
		}
	perfState.counterValues[i]=0;	
    }
	
    if(doAccumulate==0) {
        retval=ResetCounter(-1);
        if(retval) {
			cerr<<"Error resetting the event counters"<<endl;
            return 1;
        }
    }        
	
	// do nothing here
	
	for(int i=0;i<perfState.numberOfEvents;i++) {
		retval=ReadCounter(perfState.eventSet[i],ctr_value,cyc_value);
	    if(retval) {
			cerr<<"Error reading the event counters\n";
			return 1;
		}
		for(int j=0;j<noOfProcessors;j++) {
			tempCounterValues[i]+=ctr_value[j];
		}	
	}
	
 	retval=StopCounter(-1);
    if(retval) {
        cerr<<"Error stopping the event counters\n";
        return 1;
    }


// Do the Actual Counting here
	
	for(int i=0;i<perfState.numberOfEvents;i++) {
		if(!IsEventActive(perfState.eventSet[i])) {
			retval=InitEvent(perfState.eventSet[i],2);
			if(retval) {
				cerr<<"Error initialising the event "<<perfState.eventSet[i]<<" "<<EventNameFromId(perfState.eventSet[i]);
				return 1;
			}
		} else {
			ResumeEvent(perfState.eventSet[i]);
		}
	perfState.counterValues[i]=0;	
    }
	
    if(doAccumulate==0) {
        retval=ResetCounter(-1);
        if(retval) {
			cerr<<"Error resetting the event counters"<<endl;
            return 1;
        }
    }        
	
	// run the file here
	retval=Tcl_EvalFile(interp, arg);
    if(retval) {
       cerr<<"Error running the file\n";
       return 1;
    }

	
	for(int i=0;i<perfState.numberOfEvents;i++) {
		retval=ReadCounter(perfState.eventSet[i],ctr_value,cyc_value);
	    if(retval) {
			cerr<<"Error reading the event counters\n";
			return 1;
		}
		for(int j=0;j<noOfProcessors;j++) {
			perfState.counterValues[i]+=ctr_value[j];
		}	
	}

	retval=StopCounter(-1);
    if(retval) {
        cerr<<"Error stopping the event counters\n";
        return 1;
    }


	
    result=Tcl_NewListObj(0,NULL);
    
    while(counter<perfState.numberOfEvents) {
        event_name=Tcl_NewStringObj((const char *)perfState.eventsData[counter],-1);
        retval=Tcl_ListObjAppendElement(interp, result,event_name);
        if(retval) {
            cerr<<"Error adding event name to tcl list obj\n";
            return 1;
        }
        counter_value=Tcl_NewWideIntObj(perfState.counterValues[counter]-tempCounterValues[counter]);
        retval=Tcl_ListObjAppendElement(interp, result,counter_value);
        if(retval) {
            cerr<<"Error adding counter value to tcl list obj\n";
            return 1;
        }        
        counter++;
	}


	
	
    Tcl_SetObjResult(interp,result);
    return TCL_OK;        
} 
