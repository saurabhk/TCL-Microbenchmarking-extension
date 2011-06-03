/*
  * hello.c -- A minimal Tcl C extension.
  */

#include<stdlib.h>
#include <tcl.h>
#include <papi.h>
#include<string.h>
#include<stdio.h>

char**res;
int numberofevents=0;
int event_code;
int event_set = PAPI_NULL;



  static int 
List_Events_Cmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
 {	
  
 if (PAPI_library_init(PAPI_VER_CURRENT) != PAPI_VER_CURRENT) 
  {
     printf("Error initializing the PAPI Library");
  exit(1);
  }

char **data;
data = (char **)malloc(240 * sizeof(char*));

  // for each pointer, allocate storage for an array of char
 
  for (int  i = 0; i < 240; i++) {
  if(i%2==0)
    data[i] =(char *) malloc(100 * sizeof(char));
 else
    data[i] =(char *) malloc(500 * sizeof(char));

  }

int counter=0;
 int retval,i=(PAPI_PRESET_MASK);
PAPI_event_info_t  info;
 //Scan for all supported native events on this platform 
//printf("Name\t\t\t       Code\t   Description\n");
 do{
   retval = PAPI_get_event_info(i, &info);
  if (retval == PAPI_OK) {
strcpy( data[counter],info.symbol);
strcpy(data[counter+1],": ");
strcat(data[counter+1],info.long_descr);
strcat(data[counter+1],"\n ");
//  printf("%-1s 0x%-10x%s\n", info.symbol, info.event_code, info.long_descr);

  }
  counter+=2;
}
 while (PAPI_enum_event(&i, PAPI_ENUM_ALL) == PAPI_OK);


char*ev_list=Tcl_Merge(counter, (const char *const *)data);

Tcl_SetObjResult(interp, Tcl_NewStringObj(ev_list, -1));
	 
  return TCL_OK;
 
}

 static int 
 Hello_Cmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
 {	
 
 
 
 if (PAPI_library_init(PAPI_VER_CURRENT) != PAPI_VER_CURRENT) 
  {
     printf("Error initializing the PAPI Library");
  exit(1);
  }

  
 if (PAPI_create_eventset(&event_set) != PAPI_OK) {
    printf("Error in subroutine PAPIF_create_eventset");
	exit(1);
}


 
 char* arg=Tcl_GetString(objv[1]);

 int argc, code;
const char **argv;
code = Tcl_SplitList(interp, arg, &argc,  &argv);

char **index=(char **)argv;

  //  allocate storage for an array of pointers 
  res = (char **)malloc(300 * sizeof(char*));

  // for each pointer, allocate storage for an array of char
  int i;
  for ( i = 0; i < 300; i++) {
    res[i] =(char *) malloc(100 * sizeof(char));
  }



while(*index!=NULL)
{

if (PAPI_event_name_to_code(*index,&event_code) != PAPI_OK)
printf("error in event name: %d\n",PAPI_event_name_to_code(*index,&event_code));
res[numberofevents*3]=*index;
fflush(stdout);
PAPI_add_event(event_set, event_code);
index+=1;
numberofevents+=1;
}
 
   Tcl_SetObjResult(interp, Tcl_NewStringObj("Hardware counters initialized", -1));
     return TCL_OK;
 }


// sub-command that takes script string as the input

 static int 
 Run_Cmd1(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])  {	
 char* arg=Tcl_GetString(objv[1]);
 printf("%s",arg);
 
  if(PAPI_start(event_set)!= PAPI_OK) {
 printf("Abort After PAPIF_start: ");
 //char strerr[100];
//PAPI_perror(PAPI_start(event_set),strerr, 100);
//printf("%s",strerr);
	exit(1);
	}
	
	
//Tcl_EvalFile(interp, "test.tcl");

Tcl_EvalObjEx(interp, objv[1], TCL_EVAL_GLOBAL);

 //stop counting
 long_long values[100];


  if (PAPI_stop(event_set, values)!= PAPI_OK) {
  printf("Abort After PAPIF_stop: ");
	exit(1);
	}
 



int cnt=0, cnt2=1;
while(cnt<numberofevents) {
	sprintf(res[cnt2],"%lld",values[cnt]);
	res[cnt2+1]=(char*)"|";
	cnt++;
	cnt2+=3;
	}
	

char*result=Tcl_Merge(numberofevents*3-1, (const char *const *)res);

	 Tcl_SetObjResult(interp, Tcl_NewStringObj(result, -1));
	 
	 PAPI_cleanup_eventset(event_set);
	PAPI_destroy_eventset(&event_set);
	event_set=PAPI_NULL;
	numberofevents=0;
	
	 
     return TCL_OK;
 }


// sub-command that takes filename as input
static int 
 Run_Cmd2(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
 {	
 char* arg=Tcl_GetString(objv[1]);

 
  if(PAPI_start(event_set)!= PAPI_OK) {
 printf("Abort After PAPIF_start: ");
 //char strerr[100];
//PAPI_perror(PAPI_start(event_set),strerr, 100);
//printf("%s",strerr);
	exit(1);
	}
	
	
	Tcl_EvalFile(interp, arg);

//Tcl_EvalObjEx(interp, objv[1], TCL_EVAL_GLOBAL);

 //stop counting
 long_long values[100];


	  if (PAPI_stop(event_set, values)!= PAPI_OK) {
	  printf("Abort After PAPIF_stop: ");
          exit(1);
	  }
 



	int cnt=0, cnt2=1;

	while(cnt<numberofevents) {
        	sprintf(res[cnt2],"%lld",values[cnt]);
		res[cnt2+1]=(char *)"|";
		cnt++;
		cnt2+=3;
	}

	PAPI_cleanup_eventset(event_set);
	PAPI_destroy_eventset(&event_set);
	event_set=PAPI_NULL;

	
	
	char*result=Tcl_Merge(numberofevents*3-1, (const char *const *)res);
	Tcl_SetObjResult(interp, Tcl_NewStringObj(result, -1));
	numberofevents=0;
	return TCL_OK;
 }





 extern "C" int Perf_Init(Tcl_Interp *interp);
 int Perf_Init(Tcl_Interp *interp)
{
  // create namespace for our commands
  Tcl_Namespace *ns = Tcl_CreateNamespace(interp, "perf2", NULL, NULL);
  // tell Tcl to grab all subcommands on import
  Tcl_Export(interp, ns, "*", 0);

  // create our subcommand
  Tcl_CreateObjCommand(interp, "perf2::available_events", List_Events_Cmd, NULL, NULL);  
  Tcl_CreateObjCommand(interp, "perf2::init", Hello_Cmd, NULL, NULL);
  Tcl_CreateObjCommand(interp, "perf2::run1", Run_Cmd1, NULL, NULL);
  Tcl_CreateObjCommand(interp, "perf2::run2", Run_Cmd2, NULL, NULL);

  // create the ensemble
  Tcl_CreateEnsemble(interp, "perf2", ns, 0);

  // success
  return TCL_OK;
}
