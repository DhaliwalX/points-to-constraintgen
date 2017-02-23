; RUN: opt -analyze -load %bindir/lib/libTestPass.so -test-pass < %s | FileCheck %s

define void @f() {
    %a = alloca i32
    %b = alloca i32*
    store i32* %a, i32** %b
    ret void
}

; CHECK: 1 = & 0
; CHECK-NEXT: 3 = & 2
; CHECK-NEXT: * 3 = 1
