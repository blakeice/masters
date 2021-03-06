// ==++==
// 
//   
//    Copyright (c) 2002 Microsoft Corporation.  All rights reserved.
//   
//    The use and distribution terms for this software are contained in the file
//    named license.txt, which can be found in the root of this distribution.
//    By using this software in any fashion, you are agreeing to be bound by the
//    terms of this license.
//   
//    You must not remove this notice, or any other, from this software.
//   
// 
// ==--==
//*****************************************************************************
// File: CorPermE.H
//
// Defines the Memory and Error routines defined in the secuirty libraries. 
// These routines are mainly for the security dll and the runtime.
//
//*****************************************************************************
#ifndef _CORPERME_H_
#define _CORPERME_H_

//=============================================================================
// Error macros so we do not have to see goto's in the code
// Adds structure to where error handling and clean up code goes. Be careful
// when rethrowing EE exceptions, the routine must be cleaned up first.
//=============================================================================
typedef struct _CorError {
    HRESULT corError;
    LPSTR   corMsg;
} CorError;

#define CORTRY       HRESULT _tcorError = 0; LPSTR _tcorMsg = NULL;
#define CORTHROW(x)  {_tcorError = x; goto CORERROR;} //
#define CORCATCH(x)  goto CORCONT; \
                     CORERROR: \
                     { CorError x; x.corError = _tcorError; x.corMsg = _tcorMsg;
#define COREND       } CORCONT: //                                        

#ifdef DBG
// Debug versions for having annotated errors. Strings are removed in 
// free builds to keep size down.
#define CORTHROWMSG(x, y)  {_tcorError = x; _tcorMsg = y; goto CORERROR;} //
#else  // DBG
#define CORTHROWMSG(x, y)  {_tcorError = x; goto CORERROR;} //
#endif


#ifdef __cplusplus
extern "C" {
#endif

__inline
LPVOID WINAPI 
MallocM(size_t size)
{
    return LocalAlloc(LMEM_FIXED, size);
}

__inline
void WINAPI
FreeM(LPVOID pData)
{
    LocalFree((HLOCAL) pData);
}
    
#define WIDEN_CP CP_UTF8

// Helper macros for security logging
#define WIDEN(psz, pwsz) \
    LPCSTR _##psz = (LPCSTR) psz; \
    int _cc##psz = _##psz ? strlen(_##psz) + 1 : 0; \
    LPWSTR pwsz = (LPWSTR) (_cc##psz ? _alloca((_cc##psz) * sizeof(WCHAR)) : NULL); \
    if(pwsz) WszMultiByteToWideChar(WIDEN_CP, 0, _##psz, _cc##psz, pwsz, _cc##psz);


#define NARROW(pwsz, psz) \
    LPCWSTR _##pwsz = (LPCWSTR) pwsz; \
    int _cc##psz =  _##pwsz ? WszWideCharToMultiByte(WIDEN_CP, 0, _##pwsz, -1, NULL, 0, NULL, NULL) : 0; \
    LPSTR psz = (LPSTR) (_cc##psz ? _alloca(_cc##psz) : NULL); \
    if(psz) WszWideCharToMultiByte(WIDEN_CP, 0, _##pwsz, -1, psz, _cc##psz, NULL, NULL);


#ifdef __cplusplus
}
#endif

#endif
