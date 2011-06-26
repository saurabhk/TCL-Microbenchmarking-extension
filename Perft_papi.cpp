#include<Perft_common.h>

/*
 * Prototypes for procedures referenced only in this file:
 */

void
HandlePapiError(int errcode);

void
InitPapi() ;

void
ResetEventSet();

void
DestroyEventSet();

void
InitPapiMultiplexing();

void
StartCounting();

void
StopCounting();

void
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
 * The program is exited.
 *________________________________________________
 */
void
HandlePapiError(int errcode)
{
    fprintf(stderr, "PAPI error %d: %s\n",errcode,PAPI_strerror(errcode));
    exit(1);
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
 * Side effects:
 * program is exited in case of failure.
 *
 *________________________________________________
 */
void
InitPapi() 
{
    int retval;
    retval=PAPI_library_init(PAPI_VER_CURRENT);
    if (retval!= PAPI_VER_CURRENT && (retval > 0))    {
    fprintf(stderr,"PAPI library version mismatch!\n");
    exit(1);
    }
    if (retval < 0) {
    printf("Error initializing the PAPI Library\n");
    HandlePapiError(retval);
    }
    papiState.eventSet=PAPI_NULL;
    retval=PAPI_create_eventset(&papiState.eventSet);
    if ( retval!= PAPI_OK) {
        printf("Error in subroutine PAPI_create_eventset");
    	HandlePapiError(retval);
    }
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
void
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
    }

    papiState.eventSet=PAPI_NULL;
    papiState.numberOfEvents=0;
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

void
DestroyEventSet() 
{  
/*
 *Free all memory and data structures
 */        
    int retval=PAPI_destroy_eventset(&papiState.eventSet);
    if (retval != PAPI_OK) {
    printf("Error destroying the PAPI Eventset\n");
    HandlePapiError(retval);
    }
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

void
InitPapiMultiplexing()
{
    int retval=PAPI_multiplex_init ();
    if(retval!=PAPI_OK) {
    printf("Error in multiplexing\n");
    HandlePapiError(retval);
    }
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
void
StartCounting()
{
    int retval=PAPI_start(papiState.eventSet);
    if(retval!= PAPI_OK) {
    printf("Abort After PAPI_start: ");
    HandlePapiError(retval);
    }
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
void
StopCounting()
{
    int retval=PAPI_stop(papiState.eventSet,(long_long*)papiState.counterValues);
    if(retval!= PAPI_OK) {
    printf("Abort After PAPI_stop: ");
    HandlePapiError(retval);
    }
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
void
AccumCounting()
{
    int retval=PAPI_accum(papiState.eventSet,(long_long*)papiState.counterValues);
    if(retval!= PAPI_OK) {
    printf("Abort After PAPI_accum: ");
    HandlePapiError(retval);
    }
    retval=PAPI_stop(papiState.eventSet,NULL);
    if(retval!= PAPI_OK) {
    printf("Abort After PAPI_stop: ");
    HandlePapiError(retval);
    }
}

