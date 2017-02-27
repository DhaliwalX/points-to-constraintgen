#include "llvm/Pass.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/CommandLine.h"

#include "ConstraintBuilder.h"
#include "DotGraph.h"

#include <vector>
#include <utility>
#include <map>

using namespace llvm;
using namespace ptsto;

namespace {

struct BasicPointsToAnalysis : public ModulePass {
    static char ID;

    std::map<NodeIndex, NodeIndex> solvedConstraints_;
    ConstraintBuilder builder_;

    void print(raw_ostream &os, const Module */**/) const override {
        // print mapping
        os << "\nMapping:\n";
        PointsToNode::table_->dump(os);
    }

    BasicPointsToAnalysis() : ModulePass(ID) { }

    void solveConstraint(Constraint &c) {
        switch (c.getType()) {
            case ConstraintType::kAddressOf:
                solvedConstraints_.insert({ c.getDestination(), c.getSource() });
                break;

            default:
                errs() << "Not handled\n";
        }
    }

    bool runOnModule(Module &module) override {
        ConstraintBuilder builder;

        for (Function &f : module) {
            for (BasicBlock &bb : f) {
                for (Instruction &i : bb) {
                    Constraint c = builder.getConstraint(&i);
                    if (c == kInvalidConstraint) {
                        continue;
                    }

                    solveConstraint(c);
                }
            }
        }

        return false;
    }
};

}

char BasicPointsToAnalysis::ID = 0;
static RegisterPass<BasicPointsToAnalysis> X("test-pass", "A simple test Pass",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);
