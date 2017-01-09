#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/Support/raw_ostream.h"
#include "PointsToVisitor.h"

using namespace llvm;

namespace {
struct Simple : public ModulePass, public AAResultBase<Simple> {
    static char ID;
    Simple() : ModulePass(ID) {}

    bool runOnModule(Module &M) override {
        lfcpa::PointsToVisitor visitor;

        for (auto &function : M) {
            visitor.visit(function);
        }

        visitor.dump();
        return false;
    }
};
}

char Simple::ID = 0;
static RegisterPass<Simple> X("simple", "Simple Pass",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);