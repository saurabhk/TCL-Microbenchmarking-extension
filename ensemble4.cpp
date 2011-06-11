/*
  * hello.c -- A minimal Tcl C extension.
  */

#include<stdlib.h>
#include <tcl.h>
#include <papi.h>
#include<string.h>
#include<stdio.h>
#include<iostream>
using namespace std;

char**res;
int numberofevents=0;
int event_code;
int event_set = PAPI_NULL;
 long_long values[100];

void handle_error(int errcode)
{
char strerr[100];
PAPI_perror(errcode,strerr, 100);
printf("%s",strerr);
fflush(stdout);
exit(1);
}



static int 
Events_Widget_Cmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
 {	
int error=Tcl_EvalFile(interp, "widget.tcl");
//cout<<error;
//cout<<Tcl_GetStringResult(interp);
//fflush(stdout);
if(error)
{
cout<<"error code: "<<error<<"exiting";
exit(1);
}
 Tcl_SetObjResult(interp, Tcl_NewStringObj("success!", -1));
  return TCL_OK;
}

  
 static int 
 Hello_Cmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
 {	
 
  
 
PAPI_option_t options;
memset(&options,0x0,sizeof(options));
options.domain.eventset = event_set;
options.domain.domain = PAPI_MULTIPLEX;




if ((errcode=PAPI_set_opt(PAPI_DOMAIN, &options) )!= PAPI_OK)
handle_error(errcode);





/*int num;
if ((num = PAPI_get_opt(PAPI_MAX_HWCTRS,NULL)) <= 0)
handle_error(num);
printf("This machine has %d counters",num);
fflush(stdout);
 */
 





 
 
   Tcl_SetObjResult(interp, Tcl_NewStringObj("Hardware counters initialized", -1));
     return TCL_OK;
 }



