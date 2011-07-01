#include<Perft_common.h>
#include<string.h>

/*
 * Prototypes for procedures referenced only in this file:
 */
int 
RunFileCmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);

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
        fprintf(stderr,"Usage: perft::run_file filename");
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
    
    retval=StartCounting();
    if(retval==1) {
        return 1;
    }    

    retval=Tcl_EvalFile(interp, arg);
    if(retval) {
        printf("Error running the script\n");
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
        
    result=Tcl_NewListObj(0,NULL);        
    
    while(counter<papiState.numberOfEvents) {
        event_name=Tcl_NewStringObj((const char *)papiState.eventsData[counter],-1);
        retval=Tcl_ListObjAppendElement(interp, result,event_name);
        if(retval) {
            printf("Error adding event name to tcl list obj\n");
            return 1;
        }
        counter_value=Tcl_NewWideIntObj(papiState.counterValues[counter]);
        retval=Tcl_ListObjAppendElement(interp, result,counter_value);
        if(retval) {
            printf("Error adding counter value to tcl list obj\n");
            return 1;
        }        
        counter++;
	}

    Tcl_SetObjResult(interp,result);
    return TCL_OK;         
}    

