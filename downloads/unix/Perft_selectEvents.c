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
    int retval;
	char autoPath[]="auto_path";
	char* dirList=(char*)Tcl_GetVar(interp,autoPath, 0);
	int i,NoOfLocations;
	char**dirArray;
    char path[1000];
	retval=Tcl_SplitList(interp, dirList, &NoOfLocations, (const char***)&dirArray);

	for(i=0;i<NoOfLocations;i++) {
		strcpy(path,dirArray[i]);
		strcat(path,"/Perft1.0/PerftWidget.tcl");
        fprintf(stderr,"%s\n",path);
		retval=Tcl_EvalFile(interp, path);
		if(!retval) {
			break;
		}
	}
	if(i>=NoOfLocations) {
			fprintf(stderr,"Error running the widget. Set auto_path variable to include the Perft package directory \n");
			return 1;
		}	

    Tcl_Free((char *)dirArray);  
    Tcl_SetObjResult(interp, Tcl_NewStringObj("success!", -1));
    return TCL_OK;
}
