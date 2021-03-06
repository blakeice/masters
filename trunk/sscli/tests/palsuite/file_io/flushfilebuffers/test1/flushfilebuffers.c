/*=====================================================================
**
** Source:  FlushFileBuffers.c
**
** Purpose: Tests the PAL implementation of the FlushFileBuffers function
** This tests checks the return values of FlushFileBuffers -- once on an
** open handle and once on a closed handle.  
**  
** Depends:
**       CreateFile
**       WriteFile
**       CloseHandle
**       DeleteFileA
**
** 
**  Copyright (c) 2002 Microsoft Corporation.  All rights reserved.
** 
**  The use and distribution terms for this software are contained in the file
**  named license.txt, which can be found in the root of this distribution.
**  By using this software in any fashion, you are agreeing to be bound by the
**  terms of this license.
** 
**  You must not remove this notice, or any other, from this software.
** 
**
**===================================================================*/

#include <palsuite.h>


int __cdecl main(int argc, char **argv)
{
    
    int TheReturn;
    HANDLE TheFileHandle;
    DWORD temp;
    DWORD originalSize=10000;
    DWORD finalSize=10000;
    const char* fileName="the_file";

    /*                          1         2         3         4*/
    char * SomeText = "1234567890123456789012345678901234567890";
    

    if (0 != PAL_Initialize(argc,argv))
    {
        return FAIL;
    }
  
    
    /* Open the file to get a HANDLE */    
    TheFileHandle = 
        CreateFile(
            fileName,                 
            GENERIC_READ|GENERIC_WRITE,  
            FILE_SHARE_READ,                           
            NULL,                        
            OPEN_ALWAYS,                 
            FILE_ATTRIBUTE_NORMAL,       
            NULL);                       

    if(TheFileHandle == INVALID_HANDLE_VALUE) 
    {
        Fail("ERROR: CreateFile failed.  Test depends on this function.");
    }
  
    /* get the file size */
    originalSize = GetFileSize (TheFileHandle, NULL) ; 
    if(originalSize == INVALID_FILE_SIZE)
    {
         Fail("ERROR: call to GetFileSize faild with error "
             "The GetLastError is %d.",GetLastError());
    }
    
    /* Write something too the HANDLE.  Should be buffered */    
    TheReturn = WriteFile(TheFileHandle,     
                          SomeText,        
                           strlen(SomeText),
                          &temp,  
                          NULL);
    
    if(TheReturn == 0) 
    {
        Fail("ERROR: WriteFile failed.  Test depends on this function.");
    }

    /* Test to see that FlushFileBuffers returns a success value */
    TheReturn = FlushFileBuffers(TheFileHandle);
    if(TheReturn == 0) 
    {
        Fail("ERROR: The FlushFileBuffers function returned 0, which "
               "indicates failure, when trying to flush a valid HANDLE.  "
               "The GetLastError is %d.",GetLastError());
    }
  
    /* test if flush modified the file */
    finalSize = GetFileSize (TheFileHandle, NULL) ; 
    if(finalSize==INVALID_FILE_SIZE)
    {
        Fail("ERROR: call to GetFileSize faild with error "
             "The GetLastError is %d.",GetLastError());
    }
    if(finalSize!=(originalSize+strlen(SomeText)))
    {
        Fail("ERROR: FlushFileBuffers failed. data was not written to the file");
    }
  
  
    /* Close the Handle */
    TheReturn = CloseHandle(TheFileHandle);
    if(TheReturn == 0) 
    {
        Fail("ERROR: CloseHandle failed.  This function depends "
               "upon it.");
    }
    

    /* Test to see that FlushFileBuffers returns a failure value */
    TheReturn = FlushFileBuffers(TheFileHandle);
    if(TheReturn != 0) 
    {
        Fail("ERROR: The FlushFileBuffers function returned non-zero, "
               "which indicates success, when trying to flush an invalid "
               "HANDLE.");
    }  

    /* make sure file does not exist */
    if(DeleteFileA(fileName)== 0 )
    {
         Fail("ERROR: call to DeleteFileA faild with error "
             "The GetLastError is %d.",GetLastError());
    }
    PAL_Terminate();
    return PASS;
}

