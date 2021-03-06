/*=====================================================================
**
** Source:  test2.c
**
** Purpose:  Call the _gcvt function on a number of cases.  Check that it
** handles negatives, exponents and hex digits properly.  Also check that 
** the 'digit' specification works. (And that it doesn't truncate negative
** signs or decimals)  
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

struct testCase
{
    double Value;
    int Digits;
    char WinCorrectResult[128];
    char BsdCorrectResult[128]; /* for the odd case where bsd sprintf 
                                    varies from windows sprintf */
};

int __cdecl main(int argc, char **argv)
{
    char result[128];
    int i=0;

    struct testCase testCases[] =
    {
        {1234567,  7, "1234567"},
        {1234.123, 7, "1234.123"},
        {1234.1234, 7, "1234.123"},
        {12.325678e+2, 7, "1232.568"},
        {-12.3233333, 8, "-12.323333"},
        {-12.32, 8, "-12.32"},
        {-12.32e+2, 8, "-1232.", "-1232" },
        {0x21DDFABC, 8, "5.6819577e+008", "5.6819577e+08" },
        {123456789012345, 15, "123456789012345" },
        {12340000, 8, "12340000"},
        {12340000000000000, 15, "1.234e+016", "1.234e+16" },
        {12340000000000000, 17, "12340000000000000"  },
        
    };

    if (0 != (PAL_Initialize(argc, argv)))
    {
        return FAIL;
    }

    /* Loop through each case. Call _gcvt on each test case and check the
       result.
    */

    for(i = 0; i < sizeof(testCases) / sizeof(struct testCase); i++)
    {
        _gcvt(testCases[i].Value, testCases[i].Digits, result);

        if (strcmp(testCases[i].WinCorrectResult, result) != 0 && 
            
            ( testCases[i].BsdCorrectResult && 
              strcmp(testCases[i].BsdCorrectResult, result) != 0 ) )
        {
            Fail("ERROR: _gcvt attempted to convert %f with %d digits "
                 "signifigant, which resulted in "
                 "the string '%s' instead of the correct(Win) string '%s' or the"
                 "correct(bsd) string '%s'.\n",
                 testCases[i].Value,
                 testCases[i].Digits,
                 result,
                 testCases[i].WinCorrectResult,
                 testCases[i].BsdCorrectResult);
        }

        memset(result, '\0', 128);
    }
    PAL_Terminate();
    return PASS;
}
