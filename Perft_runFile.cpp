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
        exit(1);
    }
    
    if(objc>=3) {
        char* flag=Tcl_GetString(objv[2]);        
        if(!strcmp("-a",flag))
            doAccumulate=1;
    }
    
    if(papiState.numberOfEvents<=0){
        fprintf(stderr,"First use perft::init/perft::select_events to set the eventset\n");
        exit(1);
    }        
        
    char* arg=Tcl_GetString(objv[1]);

    int counter=0,retval;
    char*result;
    
    StartCounting();


    retval=Tcl_EvalFile(interp, arg);
    if(retval) {
        printf("Error running the script\n");
        exit(1);
    }
    
    if(doAccumulate==0)
        StopCounting();
    else
        AccumCounting();        
    
    while(counter<papiState.numberOfEvents) {
        sprintf(papiState.eventsData[3*counter+1],"%lld",papiState.counterValues[counter]);
        papiState.eventsData[3*counter+2]=(char *)"|";
        counter++;
	}

    result=Tcl_Merge(papiState.numberOfEvents*3-1, (const char *const *)papiState.eventsData);
 
    Tcl_SetResult(interp,result,TCL_DYNAMIC);
    return TCL_OK;        
}    

