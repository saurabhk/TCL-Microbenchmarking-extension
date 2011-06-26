#include<Perft_common.h>

/*
 * Prototypes for procedures referenced only in this file:
 */

char** 
AllocateStringArray(int noOfStrings,int size);

void
DeallocateStringArray(int noOfStrings,char**memPtr);

 /*
 *______________________________________________
 *
 * AllocateStringArray --
 *
 * This function allocates dynamic memory for 
 * an array of strings where 'size' is the size of each string
 * 
 *
 * Results:
 * Dynamic memory allocation for string array
 *________________________________________________
 */
char** 
AllocateStringArray(int noOfStrings,int size,char*** memPtr)
{
    *memPtr = (char**)malloc(noOfStrings * sizeof(char*));
    for (int  i = 0; i < noOfStrings; i++) {
        (*memPtr)[i] =(char *) malloc(size* sizeof(char));
   }
}

 /*
 *______________________________________________
 *
 * DeallocateStringArray --
 *
 * This function deallocates dynamic memory of 
 * an array of strings where 'noOfStrings' is the no. of strings
 * 
 *
 * Results:
 * Dynamic memory deallocation for string array
 *________________________________________________
 */
void
DeallocateStringArray(int noOfStrings,char***memPtr)
{
    for (int  i = 0; i < noOfStrings; i++) {
        free((*memPtr)[i]);
     }
     free(*memPtr);
}
