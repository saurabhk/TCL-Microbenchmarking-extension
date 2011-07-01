#include<Perft_common.h>

/*
 * Prototypes for procedures referenced only in this file:
 */
int 
SelectEventsCmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);

 /*
 *______________________________________________
 *
 * SelectEventsCmd --
 *
 * This command is meant to make the event selection easy by providing a GUI using tk.
 *  As soon as this function is called, a tk widget is created which allows users to choose
 * among the possible hardware events comfortably.
 *
 * 
 * Results:
 * Eventset is initialized.
 *________________________________________________
 */
 
int 
SelectEventsCmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
 {	
    int retval=Tcl_EvalFile(interp, "./lib/widget.tcl");
    if(retval) {
        printf("Error running the widget\n");
        return 1;
    }
    Tcl_SetObjResult(interp, Tcl_NewStringObj("success!", -1));
    return TCL_OK;
}
