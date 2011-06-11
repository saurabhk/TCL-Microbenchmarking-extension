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
        exit(1);
    }    
    papiState.eventsData=AllocateStringArray(3*PAPI_MAX_EVENTS,PAPI_MAX_STRING_LENGTH);
    int retval,eventCode;
    char* arg=Tcl_GetString(objv[1]);
    int argc, eventList;
    const char **argv;
    eventList= Tcl_SplitList(interp, arg, &argc,  &argv);
    char **index=(char **)argv;   
    
    papiState.eventSet=PAPI_NULL;
    papiState.numberOfEvents=0;
    
    InitPapi();
    
    InitPapiMultiplexing();
    
    retval=PAPI_assign_eventset_component(papiState.eventSet, 0);
    if ( retval!= PAPI_OK) {
        printf("Error in subroutine PAPI_assign_eventset_component");    
        HandlePapiError(retval);
    }    
    
    retval=PAPI_set_multiplex(papiState.eventSet);
    if ( retval!= PAPI_OK) {
        printf("Error in subroutine PAPI_set_multiplex");    
        HandlePapiError(retval);
    }
    
    while(*index!=NULL) {
            retval=PAPI_event_name_to_code(*index,&eventCode);
            if ( retval!= PAPI_OK) {
                printf("Error in getting information about event name: %s\n",*index);
                HandlePapiError(retval);
            }
            papiState.eventsData[papiState.numberOfEvents*3]=*index;
            retval=PAPI_add_event(papiState.eventSet, eventCode);
            if(retval!=PAPI_OK) {
                    printf("Error in adding event: %s\n",*index);
                    HandlePapiError(retval);
             }
        index+=1;
        papiState.numberOfEvents+=1;
    }
 
    Tcl_SetObjResult(interp, Tcl_NewStringObj("Hardware counters initialized", -1));
    return TCL_OK;        
}    

