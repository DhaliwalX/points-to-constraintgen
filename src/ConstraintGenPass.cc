#include "llvm/Pass.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"
#include "Variable.h"
#include "PointerInfo.h"
#include "Constraint.h"

#include <vector>
#include <utility>
#include <map>

using namespace llvm;
using namespace pass;

namespace {
struct ConstraintGen : public FunctionPass {
    std::vector<Constraint *> constraints_;

    std::map<StringRef, Variable> visited_;

    static char ID;
    
    ConstraintGen() : FunctionPass(ID) {}

    void processType1StoreInstruction(Value *pointerOperand,
            Value *valueOperand, Instruction *instruction) {
        auto info = generateInformation(valueOperand);

        if (info == nullptr) {
            errs() << "Instruction ";
            instruction->dump();
        }
        
        Variable *rhs = new Variable(valueOperand->getName(), valueOperand,
            info, true);

        info = generateInformation(pointerOperand);
        if (info == nullptr) {
            errs() << "Instruction ";
            instruction->dump();
        }
        Variable *lhs = new Variable(pointerOperand->getName(), pointerOperand,
            info, false);

        constraints_.push_back(new Constraint(lhs, rhs, instruction));
    }

    PointerInfo * generateInformation(Value *value) {
        Value *currentNode = value;
        std::vector<Value *> information;

        while (true) {
            if (auto instruction = dyn_cast<LoadInst>(currentNode)) {
                information.push_back(currentNode);
                currentNode = instruction->getPointerOperand();
            } else if (auto instruction = dyn_cast<GetElementPtrInst>(currentNode)) {
                information.push_back(currentNode);
                currentNode = instruction->getPointerOperand();
            } else if (isa<AllocaInst>(currentNode)) {
                information.push_back(currentNode);
                break;
            } else if (isa<Argument>(currentNode)) {
                information.push_back(currentNode);
                break;
            } else if (isa<CastInst>(currentNode)) {
                information.push_back(currentNode);
                currentNode = dyn_cast<Instruction>(currentNode)->getOperand(0);
            } else if (isa<CallInst>(currentNode)) {
                information.push_back(currentNode);
                break;
            } else {
                errs() << "Should not be here. Don't know how to process this"
                        << " instruction: ";
                currentNode->dump();
                errs() << "Here is information collected for instruction: ";
                value->dump();
                for (auto &value : information) {
                    value->dump();
                }
                errs() << "\n-------------------\n";
                return nullptr;
            }
        }
        return new PointerInfo(information);
    }

    void processStoreInstruction(StoreInst &instruction) {
        auto pointerOperand = instruction.getPointerOperand();
        auto valueOperand = instruction.getValueOperand();

        // we only process those Variable instructions which have pointer type
        if (!pointerOperand->getType()->getContainedType(0)->isPointerTy())
            return;
        processType1StoreInstruction(pointerOperand, valueOperand, &instruction);
    }

    void dump() {
        for (auto constraint : constraints_) {
            constraint->dump();
        }
    }

    bool runOnFunction(Function &function) override {
        for (auto &basicblock : function) {
            for (auto &instruction : basicblock) {
                if (auto store_inst = dyn_cast<StoreInst>(&instruction)) {
                    processStoreInstruction(*store_inst);
                }
            }
        }

        dump();

        return false;
    }
};
}

char ConstraintGen::ID = 0;
static RegisterPass<ConstraintGen> X("constraints", "ConstraintGen Pass",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);
