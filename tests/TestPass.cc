#include "llvm/Pass.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"

#include "ConstraintBuilder.h"
#include "DotGraph.h"

using namespace llvm;
using namespace ptsto;

namespace {

struct ConstraintGen : public ModulePass {
    static char ID;

    PointsToConstraints constraints_;

    void print(raw_ostream &os, const Module */**/) const override {
        constraints_.dump(os);
        // print mapping
        os << "\nMapping:\n";
        PointsToNode::table_->dump(os);
    }

    ConstraintGen() : ModulePass(ID) { }

    bool runOnModule(Module &module) override {
        ConstraintBuilder builder;

        for (Function &f : module) {
            for (BasicBlock &bb : f) {
                 for (Instruction &i : bb) {
                    Constraint c = builder.getConstraint(&i);
                    if (c == kInvalidConstraint) {
                        continue;
                    }

                    constraints_.push_back(c);
                }
            }
        }
        return false;
    }
};

}

char ConstraintGen::ID = 0;
static RegisterPass<ConstraintGen> X("test-pass", "A simple test Pass",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);
