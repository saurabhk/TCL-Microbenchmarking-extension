/*
  * hello.c -- A minimal Tcl C extension.
  */
  
  #include<stdlib.h>
 #include <tcl.h>
#include <papi.h>

 static int 
 Hello_Cmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
 {	
 
  if (PAPI_library_init(PAPI_VER_CURRENT) != PAPI_VER_CURRENT) 
  {
     printf("Error initializing the PAPI Library");
  exit(1);
  }
  // create the eventset
  int event_set = PAPI_NULL;
  
  if (PAPI_create_eventset(&event_set) != PAPI_OK) {
    printf("Error in subroutine PAPIF_create_eventset");
	exit(1);
}

/*int evcs[2]={PAPI_L1_DCM,PAPI_BR_INS};

   if (	PAPI_add_events(event_set, evcs,2) != PAPI_OK) {
     printf("Abort After PAPIF_add_events:3 ");
	exit(1);
}
*/

 // start counting 
PAPI_add_event(event_set, PAPI_L1_DCM);

  if(PAPI_start(event_set)!= PAPI_OK) {
 printf("Abort After PAPIF_start: ");
	exit(1);
	}
Tcl_EvalFile(interp, "test.tcl");
 //stop counting
 long_long values[2];


  if (PAPI_stop(event_set, values)!= PAPI_OK) {
  printf("Abort After PAPIF_stop: ");
	exit(1);
	}
    char str[21];
    int num=45;
    sprintf(str,"%lld",values[0]);


	printf("%s%lld\n","data misses:",values[0]);
//	cout<<"\ninstruction misses:"<<values[1]<<"\n";
    
    
     Tcl_SetObjResult(interp, Tcl_NewStringObj("hello \n world", -1));
     return TCL_OK;
 }

 /*
  * Hello_Init -- Called when Tcl loads your extension.
  */
 int DLLEXPORT
 Hello_Init(Tcl_Interp *interp)
 {
     if (Tcl_InitStubs(interp, TCL_VERSION, 0) == NULL) {
         return TCL_ERROR;
     }
     /* changed this to check for an error - GPS */
     if (Tcl_PkgProvide(interp, "Hello", "1.0") == TCL_ERROR) {
         return TCL_ERROR;
     }
     Tcl_CreateObjCommand(interp, "hello", Hello_Cmd, NULL, NULL);
     return TCL_OK;
 }
