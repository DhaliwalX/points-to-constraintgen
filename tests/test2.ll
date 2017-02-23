; RUN: opt -analyze -load %bindir/lib/libTestPass.so -test-pass < %s | FileCheck %s

define void @f() {
    %a = alloca i32*

    ret void
}

; CHECK: a1 = & a0
