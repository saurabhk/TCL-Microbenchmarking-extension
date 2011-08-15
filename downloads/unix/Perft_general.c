#include<Perft_common.h>

/*
 * Prototypes for procedures referenced only in this file:
 */
int 
HwInfoCmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);

 /*
 *______________________________________________
 *
 * HwInfoCmd --
 *
 * This function returns the number of  hardware counters 
 * available on the machine
 *________________________________________________
 */
int 
HwInfoCmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    
    char result[40]="Number of Hardware Counters:";
    char counters[10];
    int noOfCounters=PAPI_num_counters();
    if(noOfCounters<0) {
        fprintf(stderr,"Error in getting information about hardware counters: \n");
        HandlePapiError(noOfCounters);  
        return 0;      
    }
    sprintf(counters,"%d",noOfCounters);
    strcat(result,counters);
 
    Tcl_SetObjResult(interp, Tcl_NewStringObj(result, -1));
    return TCL_OK;        
}    

