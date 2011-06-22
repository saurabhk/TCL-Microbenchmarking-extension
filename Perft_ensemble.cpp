#include<Perft_common.h>

/*
 * To use C++ compiler 
 */
 extern "C" int Perft_Init(Tcl_Interp *interp);
 
 /*
 * Prototypes for procedures referenced only in this file:
 */
 
 int
 Perft_Init(Tcl_Interp *interp);
 
  /*
 *______________________________________________
 *
 * Perft_Init --
 *
 * This function creates a namespace for our commands. 
 * Moreover it declares/creates all the commands and 
 * then create an ensemble.
 *
 * Results:
 * The program is exited.
 *________________________________________________
 */
 
 int
 Perft_Init(Tcl_Interp *interp)
{
    int retval;
    /*
     * create namespace for our commands
     */
 
    Tcl_Namespace *nameSpace = Tcl_CreateNamespace(interp, "perft", NULL, NULL);

    /*
     * tell Tcl to grab all subcommands on import
     */
     
     retval=Tcl_Export(interp, nameSpace, "*", 0);
     if(retval) {
         printf("Error exporting commands\n");
         exit(1);
    }    
         
     
    /*
     * create our subcommand
     */
     
    Tcl_CreateObjCommand(interp, "perft::available_events",ListEventsCmd, NULL, NULL);  
    Tcl_CreateObjCommand(interp, "perft::init", InitCmd, NULL, NULL);
    Tcl_CreateObjCommand(interp, "perft::run_file", RunFileCmd, NULL, NULL);
    Tcl_CreateObjCommand(interp, "perft::run_script", RunScriptCmd, NULL, NULL);
    Tcl_CreateObjCommand(interp, "perft::select_events", SelectEventsCmd, NULL, NULL);
    Tcl_CreateObjCommand(interp, "perft::counters", HwInfoCmd, NULL, NULL);    

    /*
     * create the ensemble
     */
     
     Tcl_CreateEnsemble(interp, "perft", nameSpace, 0);
      return TCL_OK;
}
