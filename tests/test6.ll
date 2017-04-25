; RUN: opt -analyze -load %bindir/lib/libTestPass.so -test-pass < %s | FileCheck %s

define void @foo () {
    ; CHECK: 1 = & 0
    %a = alloca { i32*, i32 }

    ; CHECK: 3 = & 2
    %b = alloca i32

    ; CHECK: 6 = & 4 uses: { 1, 5, 5 }
    %1 = getelementptr { i32*, i32 }, {i32*, i32}* %a, i32 0, i32 0

    ; CHECK: * 6 = 3
    store i32* %b, i32** %1
    ret void
}