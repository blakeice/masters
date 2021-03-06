/*============================================================================
**
** Source:  test1.c
**
** Purpose: 
** Concatenate a few strings together, setting different lengths to be 
** used for each one.  Check to ensure the pointers which are returned are
** correct, and that the final string is what was expected.
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


int __cdecl main(int argc, char *argv[])
{
    char dest[80];
    char *test = "foo barbaz";
    char *str1 = "foo ";
    char *str2 = "bar ";
    char *str3 = "baz";
    char *ptr;
    int i;

    
    if (PAL_Initialize(argc, argv))
    {
        return FAIL;
    }


    dest[0] = 0;
    for (i=1; i<80; i++)
    {
        dest[i] = 'x';
    }

    ptr = strncat(dest, str1, strlen(str1));
    if (ptr != dest)
    {
        Fail("ERROR: Expected strncat to return ptr to %p, got %p", dest, ptr);
    }

    ptr = strncat(dest, str2, 3);
    if (ptr != dest)
    {
        Fail("ERROR: Expected strncat to return ptr to %p, got %p", dest, ptr);
    }
    if (dest[7] != 0)
    {
        Fail("ERROR: strncat did not place a terminating NULL!");
    }

    ptr = strncat(dest, str3, 20);
    if (ptr != dest)
    {
        Fail("ERROR: Expected strncat to return ptr to %p, got %p", dest, ptr);
    }
    if (strcmp(dest, test) != 0)
    {
        Fail("ERROR: Expected strncat to give \"%s\", got \"%s\"\n", 
            test, dest);
    }
    if (dest[strlen(test)+1] != 'x')
    {
        Fail("strncat went out of bounds!\n");
    }

    PAL_Terminate();

    return PASS;
}
