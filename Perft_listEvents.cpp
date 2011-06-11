#include<Perft_common.h>

/*
 * Prototypes for procedures referenced only in this file:
 */
 int 
ListEventsCmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);

 /*
 *______________________________________________
 *
 * ListEventsCmd --
 *
 * This function defines what to do if 'available_events'
 * command is called.
 * 
 * Results:
 * A list containing standard PAPI events with little description
 * of each is set as the result of the command.
 *________________________________________________
 */
int 
ListEventsCmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    InitPapi();
    char **data=AllocateStringArray(2*PAPI_MAX_EVENTS,PAPI_MAX_STRING_LENGTH);
    int counter=0;
    int retval,currentEvent=(PAPI_PRESET_MASK|0);
    PAPI_event_info_t  info;

    /*
      * Scan for all supported native events on this platform 
      */

    do
    {
        retval = PAPI_get_event_info(currentEvent, &info);
        if (retval == PAPI_OK) {
            strcpy( data[counter],info.symbol);
            strcpy(data[counter+1],info.long_descr);
        } else {
            printf("Error getting information about ");
            HandlePapiError(retval);
        }
        counter+=2;
    } while (PAPI_enum_event(&currentEvent, 10) == PAPI_OK);


    char*eventList=Tcl_Merge(counter, (const char *const *)data);
    DeallocateStringArray(240,data);
    Tcl_SetResult(interp,eventList,TCL_DYNAMIC);
	 
    return TCL_OK;
 }
