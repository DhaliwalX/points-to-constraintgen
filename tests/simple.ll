; RUN: opt -S -load %bindir/lib/libTestPass.so < %s | FileCheck %s

define void @f () {
    ret void
}

; CHECK: ret void
