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
// ===========================================================================
// File: decimal.cpp
// 
// ===========================================================================
/***
*
*Purpose:
*  This module contains the low level conversion for Decimal data type.
*
*Implementation Notes:
*
*****************************************************************************/

#include "rotor_palrt.h"

#include "oleauto.h"
#include "oautil.h"

#include "convert.h"
#include <math.h>
#include <limits.h>

#define VALIDATEDECIMAL(dec) \
    if (DECIMAL_SCALE(dec) > DECMAX || (DECIMAL_SIGN(dec) & ~DECIMAL_NEG) != 0) \
        return E_INVALIDARG;

//***********************************************************************
//
// Data tables
//


const double dblPower10[] = {
    1, 1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8, 1e9, 
    1e10, 1e11, 1e12, 1e13, 1e14, 1e15, 1e16, 1e17, 1e18, 1e19, 
    1e20, 1e21, 1e22, 1e23, 1e24, 1e25, 1e26, 1e27, 1e28, 1e29, 
    1e30, 1e31, 1e32, 1e33, 1e34, 1e35, 1e36, 1e37, 1e38, 1e39, 
    1e40, 1e41, 1e42, 1e43, 1e44, 1e45, 1e46, 1e47, 1e48, 1e49, 
    1e50, 1e51, 1e52, 1e53, 1e54, 1e55, 1e56, 1e57, 1e58, 1e59,
    1e60, 1e61, 1e62, 1e63, 1e64, 1e65, 1e66, 1e67, 1e68, 1e69, 
    1e70, 1e71, 1e72, 1e73, 1e74, 1e75, 1e76, 1e77, 1e78, 1e79,
    1e80 };

double fnDblPower10(int ix)
{
    const int maxIx = (sizeof(dblPower10)/sizeof(dblPower10[0]));
    _ASSERTE(ix >= 0);
    if (ix < maxIx)
        return dblPower10[ix];
    return pow(10.0, ix);
} // double fnDblPower10()


const DBLSTRUCT ds2to64 = DEFDS(0, 0, DBLBIAS + 65, 0);

const SPLIT64 sdlPower10[] = { {UI64(10000000000)},          // 1E10
                          {UI64(100000000000)},     // 1E11
                          {UI64(1000000000000)},    // 1E12
                          {UI64(10000000000000)},   // 1E13
                          {UI64(100000000000000)} }; // 1E14


inline wchar_t *wmemset(wchar_t *_S, wchar_t _C, size_t _N)
        {wchar_t *_Su = _S;
        for (; 0 < _N; ++_Su, --_N)
                *_Su = _C;
        return (_S); }


//***********************************************************************
//
// Conversion to/from Decimal data type
//

LPOLESTR memspn(LPOLESTR pstr, OLECHAR chr, UINT cch)
{
    for ( ; cch > 0 && *pstr == chr; cch--, pstr++);
    return pstr;
}

STDAPI 
VarDecFromI4(LONG lIn, DECIMAL FAR* pdecOut)
{
    DECIMAL_LO32(*pdecOut) = (DWORD)labs(lIn);
    DECIMAL_MID32(*pdecOut) = 0;
    DECIMAL_HI32(*pdecOut) = 0;
    DECIMAL_SIGN(*pdecOut) = (UCHAR)((lIn >> 24) & DECIMAL_NEG);
    DECIMAL_SCALE(*pdecOut) = 0;
    return NOERROR;
}

STDAPI 
VarDecFromUI4(ULONG ulIn, DECIMAL FAR* pdecOut)
{
    DECIMAL_LO32(*pdecOut) = ulIn;
    DECIMAL_MID32(*pdecOut) = 0;
    DECIMAL_HI32(*pdecOut) = 0;
    DECIMAL_SIGNSCALE(*pdecOut) = 0;
    return NOERROR;
}

STDAPI 
VarDecFromR4(float fltIn, DECIMAL FAR* pdecOut)
{
    int         iExp;    // number of bits to left of binary point
    int         iPower;
    ULONG       ulMant;
    double      dbl;
    SPLIT64     sdlLo;
    SPLIT64     sdlHi;
    int         lmax, cur;  // temps used during scale reduction

    // The most we can scale by is 10^28, which is just slightly more
    // than 2^93.  So a float with an exponent of -94 could just
    // barely reach 0.5, but smaller exponents will always round to zero.
    //
    if ( (iExp = ((SNGSTRUCT *)&fltIn)->exp - SNGBIAS) < -94 )
    {
      DECIMAL_SETZERO(*pdecOut);
      return NOERROR;
    }

    if (iExp > 96)
      return RESULT(DISP_E_OVERFLOW);

    // Round the input to a 7-digit integer.  The R4 format has
    // only 7 digits of precision, and we want to keep garbage digits
    // out of the Decimal were making.
    //
    // Calculate max power of 10 input value could have by multiplying 
    // the exponent by log10(2).  Using scaled integer multiplcation, 
    // log10(2) * 2 ^ 16 = .30103 * 65536 = 19728.3.
    //
    dbl = fabs(fltIn);
    iPower = 6 - ((iExp * 19728) >> 16);

    if (iPower >= 0) {
      // We have less than 7 digits, scale input up.
      //
      if (iPower > DECMAX)
        iPower = DECMAX;

      dbl = dbl * dblPower10[iPower];
    }
    else {
      if (iPower != -1 || dbl >= 1E7)
        dbl = dbl / fnDblPower10(-iPower);
      else 
        iPower = 0; // didn't scale it
    }

    _ASSERTE(dbl < 1E7);
    if (dbl < 1E6 && iPower < DECMAX)
    {
      dbl *= 10;
      iPower++;
      _ASSERTE(dbl >= 1E6);
    }

    // Round to integer
    //
    ulMant = (LONG)dbl;
    dbl -= (double)ulMant;  // difference between input & integer
    if ( dbl > 0.5 || dbl == 0.5 && (ulMant & 1) )
      ulMant++;

    if (ulMant == 0)
    {
      DECIMAL_SETZERO(*pdecOut);
      return NOERROR;
    }

    if (iPower < 0) {
      // Add -iPower factors of 10, -iPower <= (29 - 7) = 22.
      //
      iPower = -iPower;
      if (iPower < 10) {
        sdlLo.int64 = UInt32x32To64(ulMant, (ULONG)ulPower10[iPower]);

        DECIMAL_LO32(*pdecOut) = sdlLo.u.Lo;
        DECIMAL_MID32(*pdecOut) = sdlLo.u.Hi;
        DECIMAL_HI32(*pdecOut) = 0;
      }
      else {
        // Have a big power of 10.
        //
        if (iPower > 18) {
          sdlLo.int64 = UInt32x32To64(ulMant, (ULONG)ulPower10[iPower - 18]);
          sdlLo.int64 = UInt64x64To128(sdlLo, sdlTenToEighteen, &sdlHi.int64);

          if (sdlHi.u.Hi != 0)
            return RESULT(DISP_E_OVERFLOW);
        }
        else {
          sdlLo.int64 = UInt32x32To64(ulMant, (ULONG)ulPower10[iPower - 9]);
          sdlHi.int64 = UInt32x32To64(ulTenToNine, sdlLo.u.Hi);
          sdlLo.int64 = UInt32x32To64(ulTenToNine, sdlLo.u.Lo);
          sdlHi.int64 += sdlLo.u.Hi;
          sdlLo.u.Hi = sdlHi.u.Lo;
          sdlHi.u.Lo = sdlHi.u.Hi;
        }
        DECIMAL_LO32(*pdecOut) = sdlLo.u.Lo;
        DECIMAL_MID32(*pdecOut) = sdlLo.u.Hi;
        DECIMAL_HI32(*pdecOut) = sdlHi.u.Lo;
      }
      DECIMAL_SCALE(*pdecOut) = 0;
    }
    else {
      // Factor out powers of 10 to reduce the scale, if possible.
      // The maximum number we could factor out would be 6.  This
      // comes from the fact we have a 7-digit number, and the
      // MSD must be non-zero -- but the lower 6 digits could be
      // zero.  Note also the scale factor is never negative, so
      // we can't scale by any more than the power we used to
      // get the integer.
      //
      // DivMod32by32 returns the quotient in Lo, the remainder in Hi.
      //
      lmax = min(iPower, 6);

      // lmax is the largest power of 10 to try, lmax <= 6.
      // We'll try powers 4, 2, and 1 unless they're too big.
      //
      for (cur = 4; cur > 0; cur >>= 1)
      {
        if (cur > lmax)
          continue;

        sdlLo.int64 = DivMod32by32(ulMant, (ULONG)ulPower10[cur]);

        if (sdlLo.u.Hi == 0) {
          ulMant = sdlLo.u.Lo;
          iPower -= cur;
          lmax -= cur;
        }
      }
      DECIMAL_LO32(*pdecOut) = ulMant;
      DECIMAL_MID32(*pdecOut) = 0;
      DECIMAL_HI32(*pdecOut) = 0;
      DECIMAL_SCALE(*pdecOut) = iPower;
    }

    DECIMAL_SIGN(*pdecOut) = (char)((SNGSTRUCT *)&fltIn)->sign << 7;
    return NOERROR;
}

STDAPI 
VarDecFromR8(double dblIn, DECIMAL FAR* pdecOut)
{
    int         iExp;    // number of bits to left of binary point
    int         iPower;  // power-of-10 scale factor
    SPLIT64     sdlMant;
    SPLIT64     sdlLo;
    double      dbl;
    int         lmax, cur;  // temps used during scale reduction
    ULONG       ulPwrCur;
    ULONG       ulQuo;


    // The most we can scale by is 10^28, which is just slightly more
    // than 2^93.  So a float with an exponent of -94 could just
    // barely reach 0.5, but smaller exponents will always round to zero.
    //
    if ( (iExp = ((DBLSTRUCT *)&dblIn)->u.exp - DBLBIAS) < -94 )
    {
      DECIMAL_SETZERO(*pdecOut);
      return NOERROR;
    }

    if (iExp > 96)
      return RESULT(DISP_E_OVERFLOW);

    // Round the input to a 15-digit integer.  The R8 format has
    // only 15 digits of precision, and we want to keep garbage digits
    // out of the Decimal were making.
    //
    // Calculate max power of 10 input value could have by multiplying 
    // the exponent by log10(2).  Using scaled integer multiplcation, 
    // log10(2) * 2 ^ 16 = .30103 * 65536 = 19728.3.
    //
    dbl = fabs(dblIn);
    iPower = 14 - ((iExp * 19728) >> 16);

    if (iPower >= 0) {
      // We have less than 15 digits, scale input up.
      //
      if (iPower > DECMAX)
        iPower = DECMAX;

      dbl = dbl * dblPower10[iPower];
    }
    else {
      if (iPower != -1 || dbl >= 1E15)
        dbl = dbl / fnDblPower10(-iPower);
      else 
        iPower = 0; // didn't scale it
    }

    _ASSERTE(dbl < 1E15);
    if (dbl < 1E14 && iPower < DECMAX)
    {
      dbl *= 10;
      iPower++;
      _ASSERTE(dbl >= 1E14);
    }

    // Round to int64
    //
    sdlMant.int64 = (LONGLONG)dbl;
    dbl -= (double)(LONGLONG)sdlMant.int64;  // dif between input & integer
    if ( dbl > 0.5 || dbl == 0.5 && (sdlMant.u.Lo & 1) )
      sdlMant.int64++;

    if (sdlMant.int64 == 0)
    {
      DECIMAL_SETZERO(*pdecOut);
      return NOERROR;
    }

    if (iPower < 0) {
      // Add -iPower factors of 10, -iPower <= (29 - 15) = 14.
      //
      iPower = -iPower;
      if (iPower < 10) {
        sdlLo.int64 = UInt32x32To64(sdlMant.u.Lo, (ULONG)ulPower10[iPower]);
        sdlMant.int64 = UInt32x32To64(sdlMant.u.Hi, (ULONG)ulPower10[iPower]);
        sdlMant.int64 += sdlLo.u.Hi;
        sdlLo.u.Hi = sdlMant.u.Lo;
        sdlMant.u.Lo = sdlMant.u.Hi;
      }
      else {
        // Have a big power of 10.
        //
        _ASSERTE(iPower <= 14);
        sdlLo.int64 = UInt64x64To128(sdlMant, sdlPower10[iPower-10], &sdlMant.int64);

        if (sdlMant.u.Hi != 0)
          return RESULT(DISP_E_OVERFLOW);
      }
      DECIMAL_LO32(*pdecOut) = sdlLo.u.Lo;
      DECIMAL_MID32(*pdecOut) = sdlLo.u.Hi;
      DECIMAL_HI32(*pdecOut) = sdlMant.u.Lo;
      DECIMAL_SCALE(*pdecOut) = 0;
    }
    else {
      // Factor out powers of 10 to reduce the scale, if possible.
      // The maximum number we could factor out would be 14.  This
      // comes from the fact we have a 15-digit number, and the 
      // MSD must be non-zero -- but the lower 14 digits could be 
      // zero.  Note also the scale factor is never negative, so
      // we can't scale by any more than the power we used to
      // get the integer.
      //
      // DivMod64by32 returns the quotient in Lo, the remainder in Hi.
      //
      lmax = min(iPower, 14);

      // lmax is the largest power of 10 to try, lmax <= 14.
      // We'll try powers 8, 4, 2, and 1 unless they're too big.
      //
      for (cur = 8; cur > 0; cur >>= 1)
      {
        if (cur > lmax)
          continue;

        ulPwrCur = (ULONG)ulPower10[cur];

        if (sdlMant.u.Hi >= ulPwrCur) {
          // Overflow if we try to divide in one step.
          //
          sdlLo.int64 = DivMod64by32(sdlMant.u.Hi, ulPwrCur);
          ulQuo = sdlLo.u.Lo;
          sdlLo.u.Lo = sdlMant.u.Lo;
          sdlLo.int64 = DivMod64by32(sdlLo.int64, ulPwrCur);
        }
        else {
          ulQuo = 0;
          sdlLo.int64 = DivMod64by32(sdlMant.int64, ulPwrCur);
        }

        if (sdlLo.u.Hi == 0) {
          sdlMant.u.Hi = ulQuo;
          sdlMant.u.Lo = sdlLo.u.Lo;
          iPower -= cur;
          lmax -= cur;
        }
      }

      DECIMAL_HI32(*pdecOut) = 0;
      DECIMAL_SCALE(*pdecOut) = iPower;
      DECIMAL_LO32(*pdecOut) = sdlMant.u.Lo;
      DECIMAL_MID32(*pdecOut) = sdlMant.u.Hi;
    }

    DECIMAL_SIGN(*pdecOut) = (char)((DBLSTRUCT *)&dblIn)->u.sign << 7;
    return NOERROR;
}

STDAPI
VarDecFromCy(CY cyIn, DECIMAL FAR* pdecOut)
{
    DECIMAL_SIGN(*pdecOut) = (UCHAR)((cyIn.u.Hi >> 24) & DECIMAL_NEG);
    if (DECIMAL_SIGN(*pdecOut))
      cyIn.int64 = -cyIn.int64;

    DECIMAL_LO32(*pdecOut) = cyIn.u.Lo;
    DECIMAL_MID32(*pdecOut) = cyIn.u.Hi;
    DECIMAL_SCALE(*pdecOut) = 4;
    DECIMAL_HI32(*pdecOut) = 0;
    return NOERROR;
}

STDAPI 
VarI1FromDec(DECIMAL FAR* pdecIn, CHAR FAR* pcValOut)
{
    ULONG    ul;
    double   dbl;

    VALIDATEDECIMAL(*pdecIn); // E_INVALIDARG check
    //DebChkDec(*pdecIn);

    if (DECIMAL_SCALE(*pdecIn) == 0) {
      // Easy case: no scale factor.
      //
      if (DECIMAL_MID32(*pdecIn) != 0 || DECIMAL_HI32(*pdecIn) != 0)
        return RESULT(DISP_E_OVERFLOW);

      ul = DECIMAL_LO32(*pdecIn);
      if (ul >= 0x80 && (ul != 0x80 || !DECIMAL_SIGN(*pdecIn)))
        return RESULT(DISP_E_OVERFLOW);

      if (DECIMAL_SIGN(*pdecIn))
        ul = -(long)ul;

      *pcValOut = (char)ul;
      return NOERROR;
    }

    VarR8FromDec(pdecIn, &dbl);  // no errors possible
    return VarI1FromR8(dbl, pcValOut);
}

STDAPI 
VarUI1FromDec(DECIMAL FAR* pdecIn, BYTE FAR* pbValOut)
{
    double   dbl;
    ULONG    ul;

    VALIDATEDECIMAL(*pdecIn); // E_INVALIDARG check
    //DebChkDec(*pdecIn); 

    if (DECIMAL_SCALE(*pdecIn) == 0) {
      // Easy case: no scale factor.
      //
      ul = DECIMAL_LO32(*pdecIn);
      if (DECIMAL_MID32(*pdecIn) != 0 || DECIMAL_HI32(*pdecIn) != 0 || (ul & 0xFFFFFF00) ||
          DECIMAL_SIGN(*pdecIn) && ul != 0)
        return RESULT(DISP_E_OVERFLOW);

      *pbValOut = (BYTE)ul;
      return NOERROR;
    }

    VarR8FromDec(pdecIn, &dbl);  // no errors possible
    return VarUI1FromR8(dbl, pbValOut);
}

STDAPI 
VarI2FromDec(DECIMAL FAR* pdecIn, SHORT FAR* psValOut)
{
    ULONG    ul;
    double   dbl;

    VALIDATEDECIMAL(*pdecIn); // E_INVALIDARG check
    //DebChkDec(*pdecIn);

    if (DECIMAL_SCALE(*pdecIn) == 0) {
      // Easy case: no scale factor.
      //
      if (DECIMAL_MID32(*pdecIn) != 0 || DECIMAL_HI32(*pdecIn) != 0)
        return RESULT(DISP_E_OVERFLOW);

      ul = DECIMAL_LO32(*pdecIn);
      if (ul >= 0x8000 && (ul != 0x8000 || !DECIMAL_SIGN(*pdecIn)))
        return RESULT(DISP_E_OVERFLOW);

      if (DECIMAL_SIGN(*pdecIn))
        ul = -(long)ul;

      *psValOut = (short)ul;
      return NOERROR;
    }

    VarR8FromDec(pdecIn, &dbl);  // no errors possible
    return VarI2FromR8(dbl, psValOut);
}

STDAPI 
VarUI2FromDec(DECIMAL FAR* pdecIn, USHORT FAR* pusValOut)
{
    double   dbl;
    ULONG    ul;

    VALIDATEDECIMAL(*pdecIn); // E_INVALIDARG check
    //DebChkDec(*pdecIn);

    if (DECIMAL_SCALE(*pdecIn) == 0) {
      // Easy case: no scale factor.
      //
      ul = DECIMAL_LO32(*pdecIn);
      if (DECIMAL_MID32(*pdecIn) != 0 || DECIMAL_HI32(*pdecIn) != 0 || (ul & 0xFFFF0000) ||
          DECIMAL_SIGN(*pdecIn) && ul != 0)
        return RESULT(DISP_E_OVERFLOW);

      *pusValOut = (USHORT)ul;
      return NOERROR;
    }

    VarR8FromDec(pdecIn, &dbl);  // no errors possible
    return VarUI2FromR8(dbl, pusValOut);
}

STDAPI 
VarI4FromDec(DECIMAL FAR* pdecIn, LONG FAR* plValOut)
{
    ULONG    ul;
    double   dbl;

    VALIDATEDECIMAL(*pdecIn); // E_INVALIDARG check
    //DebChkDec(*pdecIn);

    if (DECIMAL_SCALE(*pdecIn) == 0) {
      // Easy case: no scale factor.
      //
      if (DECIMAL_MID32(*pdecIn) != 0 || DECIMAL_HI32(*pdecIn) != 0)
        return RESULT(DISP_E_OVERFLOW);

      ul = DECIMAL_LO32(*pdecIn);
      if (ul >= 0x80000000 && (ul != 0x80000000 || !DECIMAL_SIGN(*pdecIn)))
        return RESULT(DISP_E_OVERFLOW);

      if (DECIMAL_SIGN(*pdecIn))
        ul = -(long)ul;

      *plValOut = ul;
      return NOERROR;
    }

    VarR8FromDec(pdecIn, &dbl);  // no errors possible
    return VarI4FromR8(dbl, plValOut);
}

STDAPI 
VarUI4FromDec(DECIMAL FAR* pdecIn, ULONG* pulValOut)
{
    double   dbl;

    VALIDATEDECIMAL(*pdecIn); // E_INVALIDARG check
    //DebChkDec(*pdecIn);

    if (DECIMAL_SCALE(*pdecIn) == 0) {
      // Easy case: no scale factor.
      //
      if (DECIMAL_MID32(*pdecIn) != 0 || DECIMAL_HI32(*pdecIn) != 0 || 
          DECIMAL_SIGN(*pdecIn) && DECIMAL_LO32(*pdecIn) != 0)
        return RESULT(DISP_E_OVERFLOW);

      *pulValOut = DECIMAL_LO32(*pdecIn);
      return NOERROR;
    }

    VarR8FromDec(pdecIn, &dbl);  // no errors possible
    return VarUI4FromR8(dbl, pulValOut);
}

STDAPI VarR4FromDec(DECIMAL FAR* pdecIn, float FAR* pfltOut)
{
    double   dbl;

    VALIDATEDECIMAL(*pdecIn); // E_INVALIDARG check

    // Can't overflow; no errors possible.
    //
    VarR8FromDec(pdecIn, &dbl);
    *pfltOut = (float)dbl;
    return NOERROR;
}

STDAPI VarR8FromDec(DECIMAL FAR* pdecIn, double FAR* pdblOut)
{
    SPLIT64  sdlTmp;
    double   dbl;

    VALIDATEDECIMAL(*pdecIn); // E_INVALIDARG check

    sdlTmp.u.Lo = DECIMAL_LO32(*pdecIn);
    sdlTmp.u.Hi = DECIMAL_MID32(*pdecIn);

    if ( (LONG)DECIMAL_MID32(*pdecIn) < 0 )
      dbl = (ds2to64.dbl + (double)(LONGLONG)sdlTmp.int64 +
             (double)DECIMAL_HI32(*pdecIn) * ds2to64.dbl) / fnDblPower10(DECIMAL_SCALE(*pdecIn)) ;
    else
      dbl = ((double)(LONGLONG)sdlTmp.int64 +
             (double)DECIMAL_HI32(*pdecIn) * ds2to64.dbl) / fnDblPower10(DECIMAL_SCALE(*pdecIn));

    if (DECIMAL_SIGN(*pdecIn))
      dbl = -dbl;

    *pdblOut = dbl;
    return NOERROR;
}

STDAPI VarCyFromDec(DECIMAL FAR* pdecIn, CY FAR* pcyOut)
{
    SPLIT64  sdlTmp;
    SPLIT64  sdlTmp1;
    int      scale;
    ULONG    ulPwr;
    ULONG    ul;

    VALIDATEDECIMAL(*pdecIn); // E_INVALIDARG check

    scale = DECIMAL_SCALE(*pdecIn) - 4;  // the power of 10 to divide by

    if (scale == 0) {
      // No scaling needed -- the Decimal has 4 decimal places,
      // just what Currency needs.
      //
      if ( DECIMAL_HI32(*pdecIn) != 0 ||
          (DECIMAL_MID32(*pdecIn) >= 0x80000000 &&
          (DECIMAL_MID32(*pdecIn) != 0x80000000 || DECIMAL_LO32(*pdecIn) != 0 || !DECIMAL_SIGN(*pdecIn))) )
        return RESULT(DISP_E_OVERFLOW);

      sdlTmp.u.Lo = DECIMAL_LO32(*pdecIn);
      sdlTmp.u.Hi = DECIMAL_MID32(*pdecIn);

      if (DECIMAL_SIGN(*pdecIn))
        pcyOut->int64 = -(LONGLONG)sdlTmp.int64;
      else
        pcyOut->int64 = sdlTmp.int64;
      return NOERROR;
    }

    // Need to scale to get 4 decimal places.  -4 <= scale <= 24.
    //
    if (scale < 0) {
      sdlTmp1.int64 = UInt32x32To64((ULONG)ulPower10[-scale], DECIMAL_MID32(*pdecIn));
      sdlTmp.int64 = UInt32x32To64((ULONG)ulPower10[-scale], DECIMAL_LO32(*pdecIn));
      sdlTmp.u.Hi += sdlTmp1.u.Lo;
      if (DECIMAL_HI32(*pdecIn) != 0 || sdlTmp1.u.Hi != 0 || sdlTmp1.u.Lo > sdlTmp.u.Hi)
        return RESULT(DISP_E_OVERFLOW);
    }
    else if (scale < 10) {
      // DivMod64by32 returns the quotient in Lo, the remainder in Hi.
      //
      ulPwr = (ULONG)ulPower10[scale];
      if (DECIMAL_HI32(*pdecIn) >= ulPwr)
        return RESULT(DISP_E_OVERFLOW);
      sdlTmp1.u.Lo = DECIMAL_MID32(*pdecIn);
      sdlTmp1.u.Hi = DECIMAL_HI32(*pdecIn);
      sdlTmp1.int64 = DivMod64by32(sdlTmp1.int64, ulPwr);
      sdlTmp.u.Hi = sdlTmp1.u.Lo;    // quotient to high half of result
      sdlTmp1.u.Lo = DECIMAL_LO32(*pdecIn); // extended remainder
      sdlTmp1.int64 = DivMod64by32(sdlTmp1.int64, ulPwr);
      sdlTmp.u.Lo = sdlTmp1.u.Lo;    // quotient to low half of result

      // Round result based on remainder in sdlTmp1.Hi.
      //
      ulPwr >>= 1;  // compare to power/2 (power always even)
      if (sdlTmp1.u.Hi > ulPwr || sdlTmp1.u.Hi == ulPwr && (sdlTmp.u.Lo & 1))
        sdlTmp.int64++;
    }
    else {
      // We have a power of 10 in the range 10 - 24.  These powers do
      // not fit in 32 bits.  We'll handle this by scaling 2 or 3 times,
      // first by 10^10, then by the remaining amount (or 10^9, then
      // the last bit).
      //
      // To scale by 10^10, we'll actually divide by 10^10/4, which fits
      // in 32 bits.  The second scaling is multiplied by four
      // to account for it, just barely assured of fitting in 32 bits
      // (4E9 < 2^32).  Note that the upper third of the quotient is
      // either zero or one, so we skip the divide step to calculate it.  
      // (Max 4E9 divided by 2.5E9.)
      //
      // DivMod64by32 returns the quotient in Lo, the remainder in Hi.
      //
      if (DECIMAL_HI32(*pdecIn) >= ulTenToTenDiv4) {
        sdlTmp.u.Hi = 1;                // upper 1st quotient
        sdlTmp1.u.Hi = DECIMAL_HI32(*pdecIn) - ulTenToTenDiv4;  // remainder
      }
      else {
        sdlTmp.u.Hi = 0;                // upper 1st quotient
        sdlTmp1.u.Hi = DECIMAL_HI32(*pdecIn);    // remainder
      }
      sdlTmp1.u.Lo = DECIMAL_MID32(*pdecIn);     // extended remainder
      sdlTmp1.int64 = DivMod64by32(sdlTmp1.int64, ulTenToTenDiv4);
      sdlTmp.u.Lo = sdlTmp1.u.Lo;         // middle 1st quotient

      sdlTmp1.u.Lo = DECIMAL_LO32(*pdecIn);      // extended remainder
      sdlTmp1.int64 = DivMod64by32(sdlTmp1.int64, ulTenToTenDiv4);

      ulPwr = (ULONG)(ulPower10[min(scale-10, 9)] << 2);
      sdlTmp.int64 = DivMod64by32(sdlTmp.int64, ulPwr);
      ul = sdlTmp.u.Lo;                 // upper 2nd quotient

      sdlTmp.u.Lo = sdlTmp1.u.Lo;         // extended remainder
      sdlTmp.int64 = DivMod64by32(sdlTmp.int64, ulPwr);
      sdlTmp1.u.Lo = sdlTmp.u.Hi;         // save final remainder
      sdlTmp.u.Hi = ul;                 // position high result

      if (scale >= 20) {
        ulPwr = (ULONG)(ulPower10[scale-19]);
        sdlTmp.int64 = DivMod64by32(sdlTmp.int64, ulPwr);
        sdlTmp1.u.Hi |= sdlTmp1.u.Lo;     // combine sticky bits
        sdlTmp1.u.Lo = sdlTmp.u.Hi;       // final remainder
        sdlTmp.u.Hi = 0;                // guaranteed result fits in 32 bits
      }

      // Round result based on remainder in sdlTmp1.Lo.  sdlTmp1.Hi is
      // the remainder from the first division(s), representing sticky bits.
      // Current result is in sdlTmp.
      //
      ulPwr >>= 1;  // compare to power/2 (power always even)
      if (sdlTmp1.u.Lo > ulPwr || sdlTmp1.u.Lo == ulPwr &&
	  		((sdlTmp.u.Lo & 1) || sdlTmp1.u.Hi != 0))
        sdlTmp.int64++;
    }

    if (sdlTmp.u.Hi >= 0x80000000 &&
        (sdlTmp.int64 != UI64(0x8000000000000000) || !DECIMAL_SIGN(*pdecIn)))
      return RESULT(DISP_E_OVERFLOW);

    if (DECIMAL_SIGN(*pdecIn))
      sdlTmp.int64 = -(LONGLONG)sdlTmp.int64;

    pcyOut->int64 = sdlTmp.int64;
    return NOERROR;
}

STDAPI VarBstrFromDec(DECIMAL *pdecIn, LCID lcid, ULONG dwFlags, BSTR *pbstrOut)
{
    SPLIT64   sdlLo;
    SPLIT64   sdlHi;
    SPLIT64   sdlTmp;
    OLECHAR   rgch[36+1]; // 4 groups of 9, include room for sign
    LPOLESTR  pchDig = &rgch[36+1];  // we back up one before we use it
    LPOLESTR  pchEnd = &rgch[36];
    LPOLESTR  pchDP;
    INT_PTR   cch;
    int       i;
    BSTR      bstr;
    NUMINFO * pnuminfo;

    VALIDATEDECIMAL(*pdecIn); // E_INVALIDARG check

    // Rotor currently only support these
    _ASSERTE(dwFlags == 0);
    _ASSERTE(lcid == LOCALE_NEUTRAL);

    sdlLo.u.Lo = DECIMAL_LO32(*pdecIn);
    sdlLo.u.Hi = DECIMAL_MID32(*pdecIn);
    sdlHi.u.Lo = DECIMAL_HI32(*pdecIn);
    sdlHi.u.Hi = 0;

    // Break up Decimal into 9-digit chunks by dividing by 10^9
    // and keeping the remainder.  Each 9-digit chunk fits into
    // a ULONG, which can easily be converted to a digit string.
    // Digits will be produced in reverse order.
    //
    // DivMod64by32 returns the quotient in Lo, the remainder in Hi.
    //
    do {
      if (sdlHi.u.Lo) {
        sdlTmp.int64 = DivMod64by32(sdlHi.int64, ulTenToNine);
        sdlHi.u.Lo = sdlTmp.u.Lo;  // quotient
      }
      else
        sdlTmp.u.Hi = 0;

      sdlTmp.u.Lo = sdlLo.u.Hi;
      if (sdlTmp.int64) {
        sdlTmp.int64 = DivMod64by32(sdlTmp.int64, ulTenToNine);
        sdlLo.u.Hi = sdlTmp.u.Lo;
      }

      sdlTmp.u.Lo = sdlLo.u.Lo;
      sdlTmp.int64 = DivMod64by32(sdlTmp.int64, ulTenToNine);
      sdlLo.u.Lo = sdlTmp.u.Lo;

      // The 9-digit remainder is in sdlTmp.Hi.  The quotient is
      // in sdlHi.Lo:sdlLo.int64.
      //
      for (i = 0; i < 9; i++) {
        *--pchDig = (OLECHAR)(sdlTmp.u.Hi % 10) + '0';
        sdlTmp.u.Hi /= 10;
      }
    } while (sdlLo.int64 != 0 || sdlHi.u.Lo != 0);

    _ASSERTE(pchDig > rgch);

    // Have all digits in rgch[].
    // pchDig = first digit, or "-" sign.
    // pchEnd = last digit.
    // pchDP = first digit to right of DP.
    //
    pchDP = pchEnd - DECIMAL_SCALE(*pdecIn) + 1;

    if (pchDP < pchDig) {
      // Add zeros after DP and before first digit.
      //
      wmemset(pchDP, '0', (size_t) (pchDig - pchDP));
      pchDig = pchDP;
    }
    else
      // Scan off leading zeros before DP.
      //
      pchDig = memspn(pchDig, '0', (DWORD)(pchDP - pchDig));

    // Scan off trailing zeros after DP.
    //
    while (*pchEnd == '0' && pchEnd >= pchDP)
      pchEnd--;

    // If there are any decimal places, get decimal separator and
    // leading zero flag.
    //
    cch = 1;

    if (pchDP <= pchEnd) {

      IfFailRet(GetNumInfo(lcid, dwFlags, &pnuminfo));
      cch = 2;

      // See if we need leading zero before decimal point
      //
      if (pchDig == pchDP && pnuminfo->fLeadingZero )
        *--pchDig = '0';
    }

    cch += pchEnd - pchDig;
    if (cch == 0) {
      // Fix up zero.
      //
      *--pchDig = '0';
      cch++;
    }
    else if (DECIMAL_SIGN(*pdecIn)) {
      *--pchDig = '-';
      cch++;
    }

    bstr = SysAllocStringLen(NULL, (UINT)cch);
    if (bstr == NULL)
      return RESULT(E_OUTOFMEMORY);

    *pbstrOut = bstr;
    memcpy(bstr, pchDig, (size_t)((pchDP - pchDig)*sizeof(OLECHAR)));
    if (pchDP <= pchEnd) {
      bstr += pchDP - pchDig;
      *bstr = pnuminfo->chDecimal;
      memcpy(bstr+1, pchDP, (size_t)((pchEnd - pchDP + 1)*sizeof(OLECHAR)));
    }

    return NOERROR;
}



