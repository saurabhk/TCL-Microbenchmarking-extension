#include<Perft_common.h>


/*
 * Structure to store the current state : list and count of current events etc.
 */
struct papiState_t {
    int eventSet;
    int numberOfEvents;
    int64_t counterValues[PAPI_MAX_EVENTS];
    char**eventsData;
} 
papiState={0};



/*
 * Prototypes for procedures referenced only in this file:
 */

void
HandlePapiError(int errcode);

int
InitPapi() ;

int
ResetEventSet();

int
DestroyEventSet();

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
RunScriptCmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);


int 
RunFileCmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);

/*
 *______________________________________________
 *
 * HandlePapiError --
 *
 * This function handles papi library errors. 
 * It will print error names corresponding to the
 * error code given as a int argument 'errcode'.
 *
 * Results:
 * The details of the PAPI error are displayed on standard output.
 *________________________________________________
 */
void
HandlePapiError(int errcode)
{
    fprintf(stderr, "PAPI error %d: %s\n",errcode,PAPI_strerror(errcode));
    //exit(1);p
}

/*
 *______________________________________________
 *
 * InitPapi --
 *
 * This function initializes papi library. 
 * It will print error message in case of failure and handles
 * the error.
 *
 * Results:
 * papi library is initaialized.
 *
 *
 *________________________________________________
 */
int
InitPapi() 
{
    int retval;
    retval=PAPI_library_init(PAPI_VER_CURRENT);
    if (retval!= PAPI_VER_CURRENT && (retval > 0))    {
        fprintf(stderr,"PAPI library version mismatch!\n");
        return 1;
    }
    if (retval < 0) {
        fprintf(stderr,"Error initializing the PAPI Library\n");
        HandlePapiError(retval);
        return 1;
    }
    papiState.eventSet=PAPI_NULL;
    retval=PAPI_create_eventset(&papiState.eventSet);
    if ( retval!= PAPI_OK) {
        fprintf(stderr,"Error in subroutine PAPI_create_eventset");
    	HandlePapiError(retval);
    	return 1;
    }
    return 0;
}


/*
 *______________________________________________
 *
 * ResetEventSet --
 *
 * This function resets PAPI event set. 
 * It will print error message in case of failure and handles
 * the error.
 *
 * Results:
 * papi Eventset is reset.
 *
 *________________________________________________
 */
int
ResetEventSet() 
{      
    int retval;
/*
 *Remove all events in the eventset 
 */
    retval=PAPI_cleanup_eventset(papiState.eventSet);
    if (retval != PAPI_OK) {
        fprintf(stderr,"Error cleaning up the PAPI Eventset\n");
        HandlePapiError(retval);
        return 1;
    }
    papiState.eventSet=PAPI_NULL;
    papiState.numberOfEvents=0;
    return 0;
}

/*
 *______________________________________________
 *
 * DestroyEventSet --
 *
 * This function resets PAPI event set. 
 * It will print error message in case of failure and handles
 * the error.
 *
 * Results:
 * papi Eventset is reset.
 *
 *________________________________________________
 */

int
DestroyEventSet() 
{  
/*
 *Free all memory and data structures
 */        
    int retval=PAPI_destroy_eventset(&papiState.eventSet);
    if (retval != PAPI_OK) {
        fprintf(stderr,"Error destroying the PAPI Eventset\n");
        HandlePapiError(retval);
        return 1;
    }
    return 0;
}
    
 /*
 *______________________________________________
 *
 * InitPapiMultiplexing --
 *
 * This function initializes multiplexing in hardware counting. 
 * It will print error message in case of failure and handles
 * the error.
 *
 * Results:
 * papi multiplexing is initialized.
 *________________________________________________
 */

int
InitPapiMultiplexing()
{
    int retval=PAPI_multiplex_init ();
    if(retval!=PAPI_OK) {
        fprintf(stderr,"Error in multiplexing\n");
        HandlePapiError(retval);
        return 1;
    }
    return 0;
}    

 /*
 *______________________________________________
 *
 * StartCounting --
 *
 * This function starts the hardware events' counting. 
 * It will print error message in case of failure and handles
 * the error.
 *
 * Results:
 * Hardware counters start counting
 *________________________________________________
 */
int
StartCounting()
{
    int retval=PAPI_start(papiState.eventSet);
    if(retval!= PAPI_OK) {
        fprintf(stderr,"Abort After PAPI_start: ");
        HandlePapiError(retval);
        return 1;
    }
    return 0;
}    
	
 /*
 *______________________________________________
 *
 * StopCounting --
 *
 * This function stops the hardware events' counting. 
 * It will print error message in case of failure and handles
 * the error.
 *
 * Results:
 * Hardware counters stop counting
 *________________________________________________
 */
int
StopCounting()
{
    int retval=PAPI_stop(papiState.eventSet,(long_long*)papiState.counterValues);
    if(retval!= PAPI_OK) {
        fprintf(stderr,"Abort After PAPI_stop: ");
        HandlePapiError(retval);
        return 1;
    }
    return 0;
}

/*
 *______________________________________________
 *
 * AccumCounting --
 *
 * This function stops the hardware events' counting. 
 * It will print error message in case of failure and handles
 * the error. It accummulates the counter results in 
 *papiState.counterValues.
 *
 * Results:
 * Hardware counters stop counting
 *________________________________________________
 */
int
AccumCounting()
{
    int retval=PAPI_accum(papiState.eventSet,(long_long*)papiState.counterValues);
    if(retval!= PAPI_OK) {
        fprintf(stderr,"Abort After PAPI_accum: ");
        HandlePapiError(retval);
        return 1;
    }
    retval=PAPI_stop(papiState.eventSet,NULL);
    if(retval!= PAPI_OK) {
        fprintf(stderr,"Abort After PAPI_stop: ");
        HandlePapiError(retval);
        return 1;
    }
    return 0;
}



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
        fprintf(stderr,"Usage: perft::init listOfEvents");
        return 1;
    }


    int retval,eventCode;
    char* arg=Tcl_GetString(objv[1]);
    int argc, eventList;
    const char **argv;
    eventList= Tcl_SplitList(interp, arg, &argc,  &argv);
    char **index=(char **)argv;  
    
    if(papiState.eventsData==NULL) {
        AllocateStringArray(PAPI_MAX_EVENTS,PAPI_MAX_STRING_LENGTH,&papiState.eventsData);
    } else {
        retval=PAPI_cleanup_eventset(papiState.eventSet);
        if ( retval!= PAPI_OK) {
            HandlePapiError(retval);
            return 1;
        }
        // Free all memory and data structures, EventSet must be empty
        retval=PAPI_destroy_eventset(&papiState.eventSet);
        if ( retval!= PAPI_OK) {
            HandlePapiError(retval);
            return 1;
        }
    } 
      
    papiState.eventSet=PAPI_NULL;
    papiState.numberOfEvents=0;
    
    retval=InitPapi();
    if(retval==1) {
        return 1;
    }
    
    retval=InitPapiMultiplexing();
    if(retval==1) {
        return 1;
    }    
    
    retval=PAPI_assign_eventset_component(papiState.eventSet, 0);
    if ( retval!= PAPI_OK) {
        fprintf(stderr,"Error in subroutine PAPI_assign_eventset_component");    
        HandlePapiError(retval);
        return 1;
    }    
    
    retval=PAPI_set_multiplex(papiState.eventSet);
    if ( retval!= PAPI_OK) {
        fprintf(stderr,"Error in subroutine PAPI_set_multiplex");    
        HandlePapiError(retval);
        return 1;
    }
    
    while(papiState.numberOfEvents<argc) {
        retval=PAPI_event_name_to_code(*index,&eventCode);
        if ( retval!= PAPI_OK) {
            printf("Error in getting information about event name: %s\n",*index);
            HandlePapiError(retval);
            return 1;
        }
        papiState.eventsData[papiState.numberOfEvents]=*index;
        retval=PAPI_add_event(papiState.eventSet, eventCode);
        if(retval!=PAPI_OK) {
            fprintf(stderr,"Error in adding event: %s\n",*index);
            HandlePapiError(retval);
            return 1;
        }
        index+=1;
        papiState.numberOfEvents+=1;
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
    int doAccumulate=0;
    if(objc<2) {
        fprintf(stderr,"Usage: perft::run_script script");
        return 1;
    }        
    
    if(objc>=3) {
        char* flag=Tcl_GetString(objv[2]);        
        if(!strcmp("-a",flag))
            doAccumulate=1;
    }
    
    if(papiState.numberOfEvents<=0){
        fprintf(stderr,"First use perft::init/perft::select_events to set the eventset\n");
        return 1;
    }        
            

    int counter=0,retval;
    Tcl_Obj * result;
    Tcl_Obj* event_name;
    Tcl_Obj* counter_value;
    int64_t tempCounterValues[PAPI_MAX_EVENTS];

// First estimate the overheads in using the command

    retval=StartCounting();
    if(retval==1) {
        return 1;
    }
   
   if(doAccumulate==0) {
        retval=StopCounting();
        if(retval==1) {
            return 1;
        }
    }        
    else {
        retval=AccumCounting();        
        if(retval==1) {
            return 1;
        }
    }  
    
    while(counter<papiState.numberOfEvents) {
        tempCounterValues[counter]=papiState.counterValues[counter];
        counter++;
	}


// Actual counting is done here

    retval=StartCounting();
    if(retval==1) {
        return 1;
    }
   
    retval=Tcl_EvalObjEx(interp,objv[1], TCL_EVAL_GLOBAL);

    if(doAccumulate==0) {
        retval=StopCounting();
        if(retval==1) {
            return 1;
        }
    }        
    else {
        retval=AccumCounting();        
        if(retval==1) {
            return 1;
        }
    }  
    result=Tcl_NewListObj(0,NULL);
    
    counter=0;
    while(counter<papiState.numberOfEvents) {
        event_name=Tcl_NewStringObj((const char *)papiState.eventsData[counter],-1);
        retval=Tcl_ListObjAppendElement(interp, result,event_name);
        if(retval) {
            fprintf(stderr,"Error adding event name to tcl list obj\n");
            return 1;
        }
        counter_value=Tcl_NewWideIntObj(papiState.counterValues[counter]-tempCounterValues[counter]);
        retval=Tcl_ListObjAppendElement(interp, result,counter_value);
        if(retval) {
            fprintf(stderr,"Error adding counter value to tcl list obj\n");
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
    int doAccumulate=0;
    if(objc<2) {
        fprintf(stderr,"Usage: perft::run_script script");
        return 1;
    }        
    
    if(objc>=3) {
        char* flag=Tcl_GetString(objv[2]);        
        if(!strcmp("-a",flag))
            doAccumulate=1;
    }
    
    if(papiState.numberOfEvents<=0){
        fprintf(stderr,"First use perft::init/perft::select_events to set the eventset\n");
        return 1;
    }        
            

    char* arg=Tcl_GetString(objv[1]);
    int counter=0,retval;
    Tcl_Obj * result;
    Tcl_Obj* event_name;
    Tcl_Obj* counter_value;
    int64_t tempCounterValues[PAPI_MAX_EVENTS];

// First estimate the overheads in using the command

    retval=StartCounting();
    if(retval==1) {
        return 1;
    }
   
   if(doAccumulate==0) {
        retval=StopCounting();
        if(retval==1) {
            return 1;
        }
    }        
    else {
        retval=AccumCounting();        
        if(retval==1) {
            return 1;
        }
    }  
    
    while(counter<papiState.numberOfEvents) {
        tempCounterValues[counter]=papiState.counterValues[counter];
        counter++;
	}


// Actual counting is done here

    retval=StartCounting();
    if(retval==1) {
        return 1;
    }
   
    retval=Tcl_EvalFile(interp, arg);

    if(doAccumulate==0) {
        retval=StopCounting();
        if(retval==1) {
            return 1;
        }
    }        
    else {
        retval=AccumCounting();        
        if(retval==1) {
            return 1;
        }
    }  
    result=Tcl_NewListObj(0,NULL);
    
    counter=0;
    while(counter<papiState.numberOfEvents) {
        event_name=Tcl_NewStringObj((const char *)papiState.eventsData[counter],-1);
        retval=Tcl_ListObjAppendElement(interp, result,event_name);
        if(retval) {
            fprintf(stderr,"Error adding event name to tcl list obj\n");
            return 1;
        }
        counter_value=Tcl_NewWideIntObj(papiState.counterValues[counter]-tempCounterValues[counter]);
        retval=Tcl_ListObjAppendElement(interp, result,counter_value);
        if(retval) {
            fprintf(stderr,"Error adding counter value to tcl list obj\n");
            return 1;
        }        
        counter++;
	}

    Tcl_SetObjResult(interp,result);
    return TCL_OK;   
}    

