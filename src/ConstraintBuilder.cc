#include "ConstraintBuilder.h"
#include "Adaptors.h"

#include <llvm/IR/Instructions.h>
#include <llvm/Support/raw_ostream.h>

using namespace llvm;

namespace ptsto {

PointsToConstraints &ConstraintBuilder::getConstraints() {
    return constraints_;
}

void ConstraintBuilder::addConstraint(NodeIndex l, NodeIndex r,
                                      ConstraintType type) {
    constraints_.insert(l, r, type);
}

bool ConstraintBuilder::generateForModule(Module *module) {
    bool result = false;
    // TODO: Handle the global variables here

    // run for every function in module
    for (auto &function : *module) {
        result = generateForFunction(&function);

        if (!result) {
            return false;
        }
    }

    return result;
}


bool ConstraintBuilder::generateForFunction(Function *function) {
    bool result = false;

    // add arguments to the points to store
    for (auto &argument : FunctionArguments(*function)) {
        (void)table_.createObjectNode(&argument);
    }

    // run for every basicblock
    for (auto &basicblock : *function) {
        result = generateForBasicBlock(&basicblock);

        if (!result) {
            return false;
        }
    }

    return result;
}

bool ConstraintBuilder::generateForBasicBlock(BasicBlock *basicblock) {
    bool result = false;

    for (auto &instruction : *basicblock) {
        result = generateFromInstruction(&instruction);

        if (!result) {
            return false;
        }
    }
    return true;
}

bool ConstraintBuilder::generateFromInstruction(Instruction *instruction) {
    // for simplicity using switch-case instead of visitor pattern
    switch (instruction->getOpcode()) {
        case Instruction::Alloca:
        {
            // alloca instruction returns the address of newly allocated
            // variable onto stack of type T
            // e.g.
            //      %a = alloca i32
            //   here a is lhs and alloca i32 is a temporary object whose address
            //   being assigned to a
            NodeIndex rhs = table_.createObjectNode(instruction);
            NodeIndex lhs = table_.createPointerNode(instruction);
            addConstraint(lhs, rhs, ConstraintType::kAddressOf);
            return true;
        }

        case Instruction::Call:
        case Instruction::Invoke:
        {
            // TODO
            errs() << "call/invoke instruction: ";
            instruction->dump();
            return false;
        }

        case Instruction::Ret:
        {
            //TODO
            errs() << "instruction not implemented: ";
            instruction->dump();
            return false;

        }

        case Instruction::Load:
        {
            // load instruction loads the value from an address
            // we only look for those load instruction whose resulting type
            // is a pointer type.
            //   example, %a = load i32*, i32** %b
            if (!instruction->getType()->isPointerTy()) {
                return true;
            }

            NodeIndex lhs = table_.createPointerNode(instruction);
            NodeIndex rhs = table_.getPointerNode(instruction->getOperand(0));
            addConstraint(lhs, rhs, ConstraintType::kLoad);
            return true;
        }

        case Instruction::Store:
        {
            // example, store i32* %a, i32** %b
            if (!instruction->getOperand(0)->getType()->isPointerTy()) {
                return true;
            }

            NodeIndex lhs = table_.getPointerNode(instruction->getOperand(1));
            NodeIndex rhs = table_.getNode(instruction->getOperand(0));
            addConstraint(lhs, rhs, ConstraintType::kStore);
            return true;
        }

        case Instruction::GetElementPtr:
        {
            // example, %a = getelementptr {i32,i32}, {i32,i32}* %b, i1 0, i32 1
            NodeIndex lhs = table_.getOrCreatePointerNode(instruction);
            NodeIndex rhs = table_.getNode(instruction->getOperand(0));
            addConstraint(lhs, rhs, ConstraintType::kCopy);
            return true;
        }

        case Instruction::PHI:
        {
            if (!instruction->getType()->isPointerTy())
                return true;

            auto phi = dyn_cast<PHINode>(instruction);

            NodeIndex lhs = table_.getOrCreatePointerNode(instruction);

            for (unsigned i = 0, e = phi->getNumIncomingValues(); i != e;
                i++) {
                NodeIndex rhs = table_.getNode(phi->getIncomingValue(i));
                addConstraint(lhs, rhs, ConstraintType::kCopy);
            }
            return true;
        }

        case Instruction::BitCast:
        {
            if (!instruction->getType()->isPointerTy()) {
                return true;
            }

            NodeIndex lhs = table_.getOrCreatePointerNode(instruction);
            NodeIndex rhs = table_.getNode(instruction->getOperand(0));
            addConstraint(lhs, rhs, ConstraintType::kCopy);
            return true;
        }

        case Instruction::IntToPtr:
        case Instruction::PtrToInt:
        case Instruction::VAArg:
        case Instruction::ExtractValue:
        case Instruction::InsertValue:
        case Instruction::LandingPad:
        case Instruction::Resume:
        case Instruction::AtomicRMW:
        case Instruction::AtomicCmpXchg:
        case Instruction::Select:
        default:
        {
            //TODO
            errs() << "instruction not implemented: ";
            instruction->dump();
            return true;
        }


    }
}

}
