#include<Perft_common.h>

/*
 * Prototypes for procedures referenced only in this file:
 */
 int 
InitCmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);

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
        printf("Error in subroutine PAPI_assign_eventset_component");    
        HandlePapiError(retval);
        return 1;
    }    
    
    retval=PAPI_set_multiplex(papiState.eventSet);
    if ( retval!= PAPI_OK) {
        printf("Error in subroutine PAPI_set_multiplex");    
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
            printf("Error in adding event: %s\n",*index);
            HandlePapiError(retval);
            return 1;
        }
        index+=1;
        papiState.numberOfEvents+=1;
    }

    Tcl_SetObjResult(interp, Tcl_NewStringObj("Hardware counters initialized", -1));
    return TCL_OK;        
}    

