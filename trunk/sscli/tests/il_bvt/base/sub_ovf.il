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
.assembly sub_ovf.exe{}
.class public sub_ovf {
.method public static	int32 i1(int32,int32) {
.maxstack	5
try_start:
	ldarg	0
	ldarg	1
	sub.ovf
	conv.ovf.i1
	pop
	leave	try_end
try_end:
	ldc.i4	0x0
	ret
handler_start:
	pop
	leave	done
handler_end:
done:
	ldc.i4	0x1
	ret
.try try_start to try_end catch [mscorlib]System.OverflowException handler handler_start to handler_end
}
.method public static	int32 u1(int32,int32) {
.maxstack	5
try_start:
	ldarg	0
	ldarg	1
	sub.ovf.un
	conv.ovf.u1
	pop
	leave	try_end
try_end:
	ldc.i4	0x0
	ret
handler_start:
	pop
	leave	done
handler_end:
done:
	ldc.i4	0x1
	ret
.try try_start to try_end catch [mscorlib]System.OverflowException handler handler_start to handler_end
}
.method public static	int32 i2(int32,int32) {
.maxstack	5
try_start:
	ldarg	0
	ldarg	1
	sub.ovf
	conv.ovf.i2
	pop
	leave	try_end
try_end:
	ldc.i4	0x0
	ret
handler_start:
	pop
	leave	done
handler_end:
done:
	ldc.i4	0x1
	ret
.try try_start to try_end catch [mscorlib]System.OverflowException handler handler_start to handler_end
}
.method public static	int32 u2(int32,int32) {
.maxstack	5
try_start:
	ldarg	0
	ldarg	1
	sub.ovf.un
	conv.ovf.u2
	pop
	leave	try_end
try_end:
	ldc.i4	0x0
	ret
handler_start:
	pop
	leave	done
handler_end:
done:
	ldc.i4	0x1
	ret
.try try_start to try_end catch [mscorlib]System.OverflowException handler handler_start to handler_end
}
.method public static	int32 i4(int32,int32) {
.maxstack	5
try_start:
	ldarg	0
	ldarg	1
	sub.ovf
	conv.ovf.i4
	pop
	leave	try_end
try_end:
	ldc.i4	0x0
	ret
handler_start:
	pop
	leave	done
handler_end:
done:
	ldc.i4	0x1
	ret
.try try_start to try_end catch [mscorlib]System.OverflowException handler handler_start to handler_end
}
.method public static	int32 u4(int32,int32) {
.maxstack	5
try_start:
	ldarg	0
	ldarg	1
	sub.ovf.un
	conv.ovf.u4
	pop
	leave	try_end
try_end:
	ldc.i4	0x0
	ret
handler_start:
	pop
	leave	done
handler_end:
done:
	ldc.i4	0x1
	ret
.try try_start to try_end catch [mscorlib]System.OverflowException handler handler_start to handler_end
}
.method public static	int32 i8(int64,int64) {
.maxstack	5
try_start:
	ldarg	0
	ldarg	1
	sub.ovf
	conv.ovf.i8
	pop
	leave	try_end
try_end:
	ldc.i4	0x0
	ret
handler_start:
	pop
	leave	done
handler_end:
done:
	ldc.i4	0x1
	ret
.try try_start to try_end catch [mscorlib]System.OverflowException handler handler_start to handler_end
}
.method public static	int32 u8(int64,int64) {
.maxstack	5
try_start:
	ldarg	0
	ldarg	1
	sub.ovf.un
	conv.ovf.u8
	pop
	leave	try_end
try_end:
	ldc.i4	0x0
	ret
handler_start:
	pop
	leave	done
handler_end:
done:
	ldc.i4	0x1
	ret
.try try_start to try_end catch [mscorlib]System.OverflowException handler handler_start to handler_end
}
.method public static int32 main(class [mscorlib]System.String[]) {
.entrypoint
.maxstack	10
	ldc.i4		0xFFFFFF80
	ldc.i4		0x00000002
	call	int32 sub_ovf::i1(int32,int32)
	brfalse		FAIL
	ldc.i4		0x00000000
	ldc.i4		0x00000001
	call	int32 sub_ovf::u1(int32,int32)
	brfalse		FAIL
	ldc.i4		0xFFFF8000
	ldc.i4		0x00000001
	call	int32 sub_ovf::i2(int32,int32)
	brfalse		FAIL
	ldc.i4		0x00000001
	ldc.i4		0x00000002
	call	int32 sub_ovf::u2(int32,int32)
	brfalse		FAIL
	ldc.i4		0x80000000
	ldc.i4		0x00000001
	call	int32 sub_ovf::i4(int32,int32)
	brfalse		FAIL
	ldc.i4		0x0000000F
	ldc.i4		0x00000010
	call	int32 sub_ovf::u4(int32,int32)
	brfalse		FAIL
	ldc.i8		0x8000000000000000
	ldc.i8		0x0000000000000002
	call	int32 sub_ovf::i8(int64,int64)
	brfalse		FAIL
	ldc.i8		0xFFFFFFFFFFFFFFFE
	ldc.i8		0xFFFFFFFFFFFFFFFF
	call	int32 sub_ovf::u8(int64,int64)
	brfalse		FAIL
PASS:
	ldc.i4	0x0000
	ret
FAIL:
	ldc.i4	0x0001
	ret
}
}
