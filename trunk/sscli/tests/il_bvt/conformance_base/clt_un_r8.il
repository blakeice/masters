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
.class public clt_un {
.field public static	float64 _inf
.field public static	float64 _min
.field public static	float64 _one
.field public static	float64 _zero
.field public static	float64 zero
.field public static	float64 one
.field public static	float64 max
.field public static	float64 inf
.field public static	float64 NaN
.method public static void initialize() {
.maxstack		10
	ldc.r8		float64(0xFFF0000000000000)
	stsfld	float64 clt_un::_inf
	ldc.r8		float64(0xFF7FFFFFFFFFFFFF)
	stsfld	float64 clt_un::_min
	ldc.r8		float64(0xBFF0000000000000)
	stsfld	float64 clt_un::_one
	ldc.r8		float64(0x8000000000000000)
	stsfld	float64 clt_un::_zero
	ldc.r8		float64(0x0000000000000000)
	stsfld	float64 clt_un::zero
	ldc.r8		float64(0x3FF0000000000000)
	stsfld	float64 clt_un::one
	ldc.r8		float64(0x7FEFFFFFFFFFFFFF)
	stsfld	float64 clt_un::max
	ldc.r8		float64(0x7FF0000000000000)
	stsfld	float64 clt_un::inf
	ldc.r8		float64(0x7FF8000000000000)
	stsfld	float64 clt_un::NaN
	ret
}
.method public static	int32 main(class [mscorlib]System.String[]) {
.entrypoint
.maxstack		10
	call	void clt_un::initialize()
	ldsfld	float64 clt_un::_inf
	ldsfld	float64 clt_un::_inf
	clt.un
	brtrue		FAIL
	ldsfld	float64 clt_un::_inf
	ldsfld	float64 clt_un::_min
	clt.un
	brfalse		FAIL
	ldsfld	float64 clt_un::_inf
	ldsfld	float64 clt_un::_one
	clt.un
	brfalse		FAIL
	ldsfld	float64 clt_un::_inf
	ldsfld	float64 clt_un::_zero
	clt.un
	brfalse		FAIL
	ldsfld	float64 clt_un::_inf
	ldsfld	float64 clt_un::zero
	clt.un
	brfalse		FAIL
	ldsfld	float64 clt_un::_inf
	ldsfld	float64 clt_un::one
	clt.un
	brfalse		FAIL
	ldsfld	float64 clt_un::_inf
	ldsfld	float64 clt_un::max
	clt.un
	brfalse		FAIL
	ldsfld	float64 clt_un::_inf
	ldsfld	float64 clt_un::inf
	clt.un
	brfalse		FAIL
	ldsfld	float64 clt_un::_inf
	ldsfld	float64 clt_un::NaN
	clt.un
	brfalse		FAIL
	ldsfld	float64 clt_un::_min
	ldsfld	float64 clt_un::_inf
	clt.un
	brtrue		FAIL
	ldsfld	float64 clt_un::_min
	ldsfld	float64 clt_un::_min
	clt.un
	brtrue		FAIL
	ldsfld	float64 clt_un::_min
	ldsfld	float64 clt_un::_one
	clt.un
	brfalse		FAIL
	ldsfld	float64 clt_un::_min
	ldsfld	float64 clt_un::_zero
	clt.un
	brfalse		FAIL
	ldsfld	float64 clt_un::_min
	ldsfld	float64 clt_un::zero
	clt.un
	brfalse		FAIL
	ldsfld	float64 clt_un::_min
	ldsfld	float64 clt_un::one
	clt.un
	brfalse		FAIL
	ldsfld	float64 clt_un::_min
	ldsfld	float64 clt_un::max
	clt.un
	brfalse		FAIL
	ldsfld	float64 clt_un::_min
	ldsfld	float64 clt_un::inf
	clt.un
	brfalse		FAIL
	ldsfld	float64 clt_un::_min
	ldsfld	float64 clt_un::NaN
	clt.un
	brfalse		FAIL
	ldsfld	float64 clt_un::_one
	ldsfld	float64 clt_un::_inf
	clt.un
	brtrue		FAIL
	ldsfld	float64 clt_un::_one
	ldsfld	float64 clt_un::_min
	clt.un
	brtrue		FAIL
	ldsfld	float64 clt_un::_one
	ldsfld	float64 clt_un::_one
	clt.un
	brtrue		FAIL
	ldsfld	float64 clt_un::_one
	ldsfld	float64 clt_un::_zero
	clt.un
	brfalse		FAIL
	ldsfld	float64 clt_un::_one
	ldsfld	float64 clt_un::zero
	clt.un
	brfalse		FAIL
	ldsfld	float64 clt_un::_one
	ldsfld	float64 clt_un::one
	clt.un
	brfalse		FAIL
	ldsfld	float64 clt_un::_one
	ldsfld	float64 clt_un::max
	clt.un
	brfalse		FAIL
	ldsfld	float64 clt_un::_one
	ldsfld	float64 clt_un::inf
	clt.un
	brfalse		FAIL
	ldsfld	float64 clt_un::_one
	ldsfld	float64 clt_un::NaN
	clt.un
	brfalse		FAIL
	ldsfld	float64 clt_un::_zero
	ldsfld	float64 clt_un::_inf
	clt.un
	brtrue		FAIL
	ldsfld	float64 clt_un::_zero
	ldsfld	float64 clt_un::_min
	clt.un
	brtrue		FAIL
	ldsfld	float64 clt_un::_zero
	ldsfld	float64 clt_un::_one
	clt.un
	brtrue		FAIL
	ldsfld	float64 clt_un::_zero
	ldsfld	float64 clt_un::_zero
	clt.un
	brtrue		FAIL
	ldsfld	float64 clt_un::_zero
	ldsfld	float64 clt_un::zero
	clt.un
	brtrue		FAIL
	ldsfld	float64 clt_un::_zero
	ldsfld	float64 clt_un::one
	clt.un
	brfalse		FAIL
	ldsfld	float64 clt_un::_zero
	ldsfld	float64 clt_un::max
	clt.un
	brfalse		FAIL
	ldsfld	float64 clt_un::_zero
	ldsfld	float64 clt_un::inf
	clt.un
	brfalse		FAIL
	ldsfld	float64 clt_un::_zero
	ldsfld	float64 clt_un::NaN
	clt.un
	brfalse		FAIL
	ldsfld	float64 clt_un::zero
	ldsfld	float64 clt_un::_inf
	clt.un
	brtrue		FAIL
	ldsfld	float64 clt_un::zero
	ldsfld	float64 clt_un::_min
	clt.un
	brtrue		FAIL
	ldsfld	float64 clt_un::zero
	ldsfld	float64 clt_un::_one
	clt.un
	brtrue		FAIL
	ldsfld	float64 clt_un::zero
	ldsfld	float64 clt_un::_zero
	clt.un
	brtrue		FAIL
	ldsfld	float64 clt_un::zero
	ldsfld	float64 clt_un::zero
	clt.un
	brtrue		FAIL
	ldsfld	float64 clt_un::zero
	ldsfld	float64 clt_un::one
	clt.un
	brfalse		FAIL
	ldsfld	float64 clt_un::zero
	ldsfld	float64 clt_un::max
	clt.un
	brfalse		FAIL
	ldsfld	float64 clt_un::zero
	ldsfld	float64 clt_un::inf
	clt.un
	brfalse		FAIL
	ldsfld	float64 clt_un::zero
	ldsfld	float64 clt_un::NaN
	clt.un
	brfalse		FAIL
	ldsfld	float64 clt_un::one
	ldsfld	float64 clt_un::_inf
	clt.un
	brtrue		FAIL
	ldsfld	float64 clt_un::one
	ldsfld	float64 clt_un::_min
	clt.un
	brtrue		FAIL
	ldsfld	float64 clt_un::one
	ldsfld	float64 clt_un::_one
	clt.un
	brtrue		FAIL
	ldsfld	float64 clt_un::one
	ldsfld	float64 clt_un::_zero
	clt.un
	brtrue		FAIL
	ldsfld	float64 clt_un::one
	ldsfld	float64 clt_un::zero
	clt.un
	brtrue		FAIL
	ldsfld	float64 clt_un::one
	ldsfld	float64 clt_un::one
	clt.un
	brtrue		FAIL
	ldsfld	float64 clt_un::one
	ldsfld	float64 clt_un::max
	clt.un
	brfalse		FAIL
	ldsfld	float64 clt_un::one
	ldsfld	float64 clt_un::inf
	clt.un
	brfalse		FAIL
	ldsfld	float64 clt_un::one
	ldsfld	float64 clt_un::NaN
	clt.un
	brfalse		FAIL
	ldsfld	float64 clt_un::max
	ldsfld	float64 clt_un::_inf
	clt.un
	brtrue		FAIL
	ldsfld	float64 clt_un::max
	ldsfld	float64 clt_un::_min
	clt.un
	brtrue		FAIL
	ldsfld	float64 clt_un::max
	ldsfld	float64 clt_un::_one
	clt.un
	brtrue		FAIL
	ldsfld	float64 clt_un::max
	ldsfld	float64 clt_un::_zero
	clt.un
	brtrue		FAIL
	ldsfld	float64 clt_un::max
	ldsfld	float64 clt_un::zero
	clt.un
	brtrue		FAIL
	ldsfld	float64 clt_un::max
	ldsfld	float64 clt_un::one
	clt.un
	brtrue		FAIL
	ldsfld	float64 clt_un::max
	ldsfld	float64 clt_un::max
	clt.un
	brtrue		FAIL
	ldsfld	float64 clt_un::max
	ldsfld	float64 clt_un::inf
	clt.un
	brfalse		FAIL
	ldsfld	float64 clt_un::max
	ldsfld	float64 clt_un::NaN
	clt.un
	brfalse	FAIL
	ldsfld	float64 clt_un::inf
	ldsfld	float64 clt_un::_inf
	clt.un
	brtrue		FAIL
	ldsfld	float64 clt_un::inf
	ldsfld	float64 clt_un::_min
	clt.un
	brtrue		FAIL
	ldsfld	float64 clt_un::inf
	ldsfld	float64 clt_un::_one
	clt.un
	brtrue		FAIL
	ldsfld	float64 clt_un::inf
	ldsfld	float64 clt_un::_zero
	clt.un
	brtrue		FAIL
	ldsfld	float64 clt_un::inf
	ldsfld	float64 clt_un::zero
	clt.un
	brtrue		FAIL
	ldsfld	float64 clt_un::inf
	ldsfld	float64 clt_un::one
	clt.un
	brtrue		FAIL
	ldsfld	float64 clt_un::inf
	ldsfld	float64 clt_un::max
	clt.un
	brtrue		FAIL
	ldsfld	float64 clt_un::inf
	ldsfld	float64 clt_un::inf
	clt.un
	brtrue		FAIL
	ldsfld	float64 clt_un::inf
	ldsfld	float64 clt_un::NaN
	clt.un
	brfalse		FAIL
	ldsfld	float64 clt_un::NaN
	ldsfld	float64 clt_un::_inf
	clt.un
	brfalse		FAIL
	ldsfld	float64 clt_un::NaN
	ldsfld	float64 clt_un::_min
	clt.un
	brfalse		FAIL
	ldsfld	float64 clt_un::NaN
	ldsfld	float64 clt_un::_one
	clt.un
	brfalse		FAIL
	ldsfld	float64 clt_un::NaN
	ldsfld	float64 clt_un::_zero
	clt.un
	brfalse		FAIL
	ldsfld	float64 clt_un::NaN
	ldsfld	float64 clt_un::zero
	clt.un
	brfalse		FAIL
	ldsfld	float64 clt_un::NaN
	ldsfld	float64 clt_un::one
	clt.un
	brfalse		FAIL
	ldsfld	float64 clt_un::NaN
	ldsfld	float64 clt_un::max
	clt.un
	brfalse		FAIL
	ldsfld	float64 clt_un::NaN
	ldsfld	float64 clt_un::inf
	clt.un
	brfalse		FAIL
	ldsfld	float64 clt_un::NaN
	ldsfld	float64 clt_un::NaN
	clt.un
	brfalse		FAIL
PASS:
	ldc.i4	0x0000
	ret
FAIL:
	ldc.i4	0x0001
	ret
}
}
.assembly clt_un_r8.exe{}
