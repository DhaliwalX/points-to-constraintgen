; RUN: opt -analyze -load %bindir/lib/libTestPass.so -test-pass < %s | FileCheck %s

define void @foo() {
    ; CHECK: 1 = & 0
    %a = alloca { i32*, i32 }
    %1 = load { i32*, i32 }, { i32*, i32 }* %a
    %2 = extractvalue { i32*, i32 } %1, 0
    ret void
}
