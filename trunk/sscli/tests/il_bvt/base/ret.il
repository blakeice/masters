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
.assembly ret.exe{}
.class public _ret {
.field public static native int PTR
.method public void .ctor() {
.maxstack	10
	ldarg.0
	call instance void [mscorlib]System.Object::.ctor()
	ret
}
.method public static		int32 _rem(int32) {
.maxstack	5
	ldarg	0
	ret
}
.method public static		int64 reti8(int64) {
.maxstack	5
	ldarg	0
	ret
}
.method public static		float32 retr4(float32) {
.maxstack	5
	ldarg	0
	ret
}
.method public static		float64 retr8(float64) {
.maxstack	5
	ldarg	0
	ret
}
.method public static		class _ret retref(class _ret) {
.maxstack	5
	ldarg	0
	ret
}
.method public static		int32 retI(int32*) {
.maxstack	5
	ldarg		0
	conv.i4
	ret
}
.method public static int32 main(class [mscorlib]System.String[]) {
.entrypoint
.maxstack	10
.locals	(class _ret)
	newobj		void _ret::.ctor()
	stloc	0	
        ldc.i4          0x1
        call    int32 _ret::_rem(int32)
        ldc.i4          0x1
        ceq
        brfalse         FAIL
        ldc.i8          0x2
        call    int64 _ret::reti8(int64)
        ldc.i8          0x2
        ceq
        brfalse         FAIL
        ldc.r4          1.1
        call    float32 _ret::retr4(float32)
        ldc.r4          1.1
        ceq
        brfalse         FAIL
        ldc.r8          2.2
        call    float64 _ret::retr8(float64)
        ldc.r8          2.2
        ceq
        brfalse         FAIL
        ldloc   0
        call    class _ret _ret::retref(class _ret)
        ldloc   0
        ceq
        brfalse         FAIL
        ldsflda		native int _ret::PTR
        call    int32 _ret::retI(int32*)
        ldsflda		native int _ret::PTR
	conv.i4
        ceq
        brfalse         FAIL
PASS:
	ldc.i4	0x0000
	ret
FAIL:
	ldc.i4	0x0001
	ret
}
}
