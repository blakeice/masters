/*=============================================================================
**
** Source: test1.c
**
** Purpose: Test to ensure that sinh return the correct values
** 
** Dependencies: PAL_Initialize
**               PAL_Terminate
**				 Fail
**               fabs
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
**===========================================================================*/

#include <palsuite.h>

#define DELTA 0.0000001 /*Error acceptance level to the 7th decimal */

/**
 * Helper test structure
 */
struct test
{
    double value;   /* value to test the function with */
    double result;  /* expected result */
};

/**
 * main
 * 
 * executable entry point
 */
INT __cdecl main(INT argc, CHAR **argv)
{
    double pi = 3.1415926535;
    int i;

    struct test tests[] = 
    {
        /* Value        test result */
        { 0,                0 },
        { pi/2.0,           2.3012989 },
        { pi,               11.5487394 },
        { (3.0*pi) / 2.0,   55.6543976 },
        { 2.0 * pi,         267.7448940 },
        { 5.0*pi/2.0,       1287.9850539 },
        { 3.0*pi,           6195.8238619 },
        { (7.0*pi) / 2.0,   29804.8707287 },
        { 4.0 * pi,         143375.6565151 }
    };


    /* PAL initialization */
    if( PAL_Initialize(argc, argv) != 0 )
    {
	    return (FAIL);
    }

    for( i = 0; i < sizeof(tests) / sizeof(struct test); i++)
    {
        double result;
        double testDelta;


        result = sinh( tests[i].value );

        /*
         * The test is valid when the difference between the
         * result and the expectation is less than DELTA
         */
        testDelta = fabs( result - tests[i].result );
        if( testDelta >= DELTA )
        {
            Fail( "sinh(%g) returned %g"
                  " when it should have returned %g",
                  tests[i].value,
                  result, 
                  tests[i].result );
        }
    }

    PAL_Terminate();
    return PASS;
}













