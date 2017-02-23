; RUN: opt -analyze -load %bindir/lib/libTestPass.so -test-pass < %s | FileCheck %s

define void @f() {
    %b = alloca i32*
    %1 = load i32*, i32** %b
    ret void
}

; CHECK: 1 = & 0
; CHECK: 2 = * 1
