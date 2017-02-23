; RUN: opt -analyze -load %bindir/lib/libTestPass.so -test-pass < %s | FileCheck %s

define void @f() {
    %a = alloca i32
    %b = alloca i32*
    store i32* %a, i32** %b
    ret void
}

; CHECK: a1 = & a0
; CHECK-NEXT: a3 = & a2
; CHECK-NEXT: * a3 = a1
