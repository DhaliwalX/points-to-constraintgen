#include "llvm/Pass.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"

#include "ConstraintBuilder.h"

#include <vector>
#include <utility>
#include <map>

using namespace llvm;
using namespace ptsto;

namespace {

struct ConstraintGen : public ModulePass {
    static char ID;

    ConstraintGen() : ModulePass(ID) { }

    bool runOnModule(Module &module) override {
        ConstraintBuilder builder;

        builder.generateForModule(&module);
        PointsToConstraints constraints = builder.getConstraints();

        constraints.dump();
    }
};

}

char ConstraintGen::ID = 0;
static RegisterPass<ConstraintGen> X("constraints", "ConstraintGen Pass",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);
