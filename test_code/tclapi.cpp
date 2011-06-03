#include <stdio.h>
  #include <stdlib.h>

  /*This is specifically for C++.*/
  extern "C" {
    #include <tcl.h>
  }

  /*Thanks to Kevin Kenny for his help with this.*/

  class Math {
    public:
    static int AddObjCmd (ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {
      return (reinterpret_cast<Math*>(clientData))->AddMethod (interp, objc, objv);
    }
    int AddMethod (Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
  };

  int Math::AddMethod (Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {
    long n1;
    long n2;
    Tcl_Obj *res;

    /*The command name is objv[0] so 3 arguments are expected.*/
    if (3 != objc) {
      Tcl_WrongNumArgs (interp, 1, objv, "n1 n2");
      return TCL_ERROR;
    }

    if (TCL_OK != Tcl_GetLongFromObj (interp, objv[1], &n1)) {
      /*The error result should be set by Tcl_GetLongFromObj.*/
      return TCL_ERROR;
    }

    if (TCL_OK != Tcl_GetLongFromObj (interp, objv[2], &n2)) {
      return TCL_ERROR;
    }

    res = Tcl_NewLongObj (n1 + n2);

    Tcl_SetObjResult (interp, res);

    return TCL_OK;
  }

  int main (int argc, char *argv[]) {
    Tcl_Interp *interp;

    /*This finds Tcl's library files and performs some initialization.*/
    Tcl_FindExecutable (argv[0]);

    interp = Tcl_CreateInterp ();

    if (TCL_OK != Tcl_Init (interp)) {
      fprintf (stderr, "Tcl_Init error: %s\n", Tcl_GetStringResult (interp));
      exit (EXIT_FAILURE);
    }

    Math *inst = new Math;

    Tcl_CreateObjCommand (interp, "+", inst->AddObjCmd, (ClientData) inst, (Tcl_CmdDeleteProc *) NULL);

    while (1) {
      char cmd[1024];

      fgets (cmd, sizeof (cmd), stdin);

      if (TCL_OK != Tcl_Eval (interp, cmd)) {
	fprintf (stderr, "error: %s\n", Tcl_GetStringResult (interp));
	continue;
      }

      printf ("result is: %s\n", Tcl_GetStringResult (interp));
    }

    return EXIT_SUCCESS;
  }
