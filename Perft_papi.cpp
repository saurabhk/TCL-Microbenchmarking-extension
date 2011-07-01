#include<Perft_common.h>

/*
 * Prototypes for procedures referenced only in this file:
 */

void
HandlePapiError(int errcode);

int
InitPapi() ;

int
ResetEventSet();

int
DestroyEventSet();

int
InitPapiMultiplexing();

int
StartCounting();

int
StopCounting();

int
AccumCounting();

/*
 *______________________________________________
 *
 * HandlePapiError --
 *
 * This function handles papi library errors. 
 * It will print error names corresponding to the
 * error code given as a int argument 'errcode'.
 *
 * Results:
 * The details of the PAPI error are displayed on standard output.
 *________________________________________________
 */
void
HandlePapiError(int errcode)
{
    fprintf(stderr, "PAPI error %d: %s\n",errcode,PAPI_strerror(errcode));
    //exit(1);
}

/*
 *______________________________________________
 *
 * InitPapi --
 *
 * This function initializes papi library. 
 * It will print error message in case of failure and handles
 * the error.
 *
 * Results:
 * papi library is initaialized.
 *
 *
 *________________________________________________
 */
int
InitPapi() 
{
    int retval;
    retval=PAPI_library_init(PAPI_VER_CURRENT);
    if (retval!= PAPI_VER_CURRENT && (retval > 0))    {
        fprintf(stderr,"PAPI library version mismatch!\n");
        return 1;
    }
    if (retval < 0) {
        printf("Error initializing the PAPI Library\n");
        HandlePapiError(retval);
        return 1;
    }
    papiState.eventSet=PAPI_NULL;
    retval=PAPI_create_eventset(&papiState.eventSet);
    if ( retval!= PAPI_OK) {
        printf("Error in subroutine PAPI_create_eventset");
    	HandlePapiError(retval);
    	return 1;
    }
    return 0;
}


/*
 *______________________________________________
 *
 * ResetEventSet --
 *
 * This function resets PAPI event set. 
 * It will print error message in case of failure and handles
 * the error.
 *
 * Results:
 * papi Eventset is reset.
 *
 *________________________________________________
 */
int
ResetEventSet() 
{      
    int retval;
/*
 *Remove all events in the eventset 
 */
    retval=PAPI_cleanup_eventset(papiState.eventSet);
    if (retval != PAPI_OK) {
        printf("Error cleaning up the PAPI Eventset\n");
        HandlePapiError(retval);
        return 1;
    }
    papiState.eventSet=PAPI_NULL;
    papiState.numberOfEvents=0;
    return 0;
}

/*
 *______________________________________________
 *
 * DestroyEventSet --
 *
 * This function resets PAPI event set. 
 * It will print error message in case of failure and handles
 * the error.
 *
 * Results:
 * papi Eventset is reset.
 *
 *________________________________________________
 */

int
DestroyEventSet() 
{  
/*
 *Free all memory and data structures
 */        
    int retval=PAPI_destroy_eventset(&papiState.eventSet);
    if (retval != PAPI_OK) {
        printf("Error destroying the PAPI Eventset\n");
        HandlePapiError(retval);
        return 1;
    }
    return 0;
}
    
 /*
 *______________________________________________
 *
 * InitPapiMultiplexing --
 *
 * This function initializes multiplexing in hardware counting. 
 * It will print error message in case of failure and handles
 * the error.
 *
 * Results:
 * papi multiplexing is initialized.
 *________________________________________________
 */

int
InitPapiMultiplexing()
{
    int retval=PAPI_multiplex_init ();
    if(retval!=PAPI_OK) {
        printf("Error in multiplexing\n");
        HandlePapiError(retval);
        return 1;
    }
    return 0;
}    

 /*
 *______________________________________________
 *
 * StartCounting --
 *
 * This function starts the hardware events' counting. 
 * It will print error message in case of failure and handles
 * the error.
 *
 * Results:
 * Hardware counters start counting
 *________________________________________________
 */
int
StartCounting()
{
    int retval=PAPI_start(papiState.eventSet);
    if(retval!= PAPI_OK) {
        printf("Abort After PAPI_start: ");
        HandlePapiError(retval);
        return 1;
    }
    return 0;
}    
	
 /*
 *______________________________________________
 *
 * StopCounting --
 *
 * This function stops the hardware events' counting. 
 * It will print error message in case of failure and handles
 * the error.
 *
 * Results:
 * Hardware counters stop counting
 *________________________________________________
 */
int
StopCounting()
{
    int retval=PAPI_stop(papiState.eventSet,(long_long*)papiState.counterValues);
    if(retval!= PAPI_OK) {
        printf("Abort After PAPI_stop: ");
        HandlePapiError(retval);
        return 1;
    }
    return 0;
}

/*
 *______________________________________________
 *
 * AccumCounting --
 *
 * This function stops the hardware events' counting. 
 * It will print error message in case of failure and handles
 * the error. It accummulates the counter results in 
 *papiState.counterValues.
 *
 * Results:
 * Hardware counters stop counting
 *________________________________________________
 */
int
AccumCounting()
{
    int retval=PAPI_accum(papiState.eventSet,(long_long*)papiState.counterValues);
    if(retval!= PAPI_OK) {
        printf("Abort After PAPI_accum: ");
        HandlePapiError(retval);
        return 1;
    }
    retval=PAPI_stop(papiState.eventSet,NULL);
    if(retval!= PAPI_OK) {
        printf("Abort After PAPI_stop: ");
        HandlePapiError(retval);
        return 1;
    }
    return 0;
}

