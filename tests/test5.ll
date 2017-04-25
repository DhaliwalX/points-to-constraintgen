; RUN: opt -analyze -load %bindir/lib/libTestPass.so -test-pass < %s | FileCheck %s

define void @foo () {
    ; CHECK: 1 = & 0
    %a = alloca i32
    %b = alloca i32

    store i32 10, i32* %a
    store i32 20, i32* %a

    ; %0 = load i32, i32* %a
    ; %1 = load i32, i32* %b
    ; %2 = add i32 %0, %1

    ret void
}