; RUN: opt -analyze -load %bindir/lib/libTestPass.so -test-pass < %s | FileCheck %s

%struct.S = type { i32, i32* }

define void @f1() {
    ; CHECK: 1 = & 0
    ; CHECK: 4 = & 2 uses: { 1, 3, 3 }
    %a = alloca %struct.S
    %b = getelementptr %struct.S, %struct.S* %a, i32 0, i32 0
    ret void
}