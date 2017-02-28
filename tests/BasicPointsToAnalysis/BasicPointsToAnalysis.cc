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
    PointsToConstraints constraints_;

    void print(raw_ostream &os, const Module */**/) const override {
        os << "Initial constraints:\n";
        constraints_.dump(os);

        os << "Solved constraints:\n";
        for (auto constraint : solvedConstraints_) {
            os << constraint.first << " --> " << constraint.second << "\n";
        }

        // print mapping
        os << "\nMapping:\n";
        PointsToNode::table_->dump(os);
    }

    BasicPointsToAnalysis() : ModulePass(ID) { }

    NodeIndex getPointeeofPointee(NodeIndex pointer) {        
        auto it = solvedConstraints_.find(pointer);
        assert(solvedConstraints_.end() != it);

        it = solvedConstraints_.find(it->second);
        assert(solvedConstraints_.end() != it);
        return it->second;
    }

    void solveConstraint(Constraint &c) {
        switch (c.getType()) {
            case ConstraintType::kAddressOf:
                solvedConstraints_.insert({ c.getDestination(), c.getSource() });
                break;

            case ConstraintType::kLoad: {
                // a = *b
                // a --> x  & b --> y --> z ==> a --> z
                auto p = getPointeeofPointee(c.getSource());
                solvedConstraints_.insert({ c.getDestination(), p});
                break;
            }
            case ConstraintType::kStore: {
                // *a = b
                // a--> x --> y & b --> z ==> x --> z                
                auto it = solvedConstraints_.find(c.getDestination());
                assert(solvedConstraints_.end() != it);
                solvedConstraints_[it->second] = solvedConstraints_[c.getSource()];
                break;
            }

            case ConstraintType::kCopy: {
                // a = b
                // a --> x & b --> y ==> a --> y

            }
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

                    // save the initial constraints
                    constraints_.push_back(c);
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
