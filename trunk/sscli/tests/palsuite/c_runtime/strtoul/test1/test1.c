/*============================================================================
**
** Source:  test1.c
**
** Purpose: 
** Tests stroul with different bases and overflows, as well as valid input.  
** Makes sure that the end pointer is correct.
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
**==========================================================================*/

#include <palsuite.h>

char teststr1[] = "12345";
char teststr2[] = "Z";
char teststr3[] = "4294967295";
char teststr4[] = "4294967296";

typedef struct
{
    char *str;
    char *end;
    int base;
    unsigned long result;
} TestCase;

TestCase TestCases[] = 
{
    { teststr1, teststr1 + 3, 4, 27},
    { teststr1, teststr1 + 5, 10, 12345},
    { teststr2, teststr2, 10, 0},
    { teststr3, teststr3+10, 10, 4294967295ul},
    { teststr4, teststr4+10, 10, 4294967295ul}
};

int NumCases = sizeof(TestCases) / sizeof(TestCases[0]);


int __cdecl main(int argc, char *argv[])
{
    char *end;
    unsigned long l;
    int i;
    
    if (PAL_Initialize(argc, argv))
    {
        return FAIL;
    }


    for (i=0; i<NumCases; i++)
    {
        l = strtoul(TestCases[i].str, &end, TestCases[i].base);

        if (l != TestCases[i].result)
        {
            Fail("ERROR: Expected strtoul to return %d, got %d\n", 
                TestCases[i].result, l);
        }

        if (end != TestCases[i].end)
        {
            Fail("ERROR: Expected strtoul to give an end value of %p, got %p\n",
                TestCases[i].end, end);
        }
    }
    PAL_Terminate();
    return PASS;
}
