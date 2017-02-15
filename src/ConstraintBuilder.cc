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

    for (auto &global : module->globals()) {
        (void)getTable().createPointerNode(&global);
    }

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
        (void)PointsToNode::table_->createObjectNode(&argument);
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

void ConstraintBuilder::processGetElementPtrNode(GetElementPtrInst *instruction)
{
    NodeIndex id;

    User::op_iterator i = instruction->idx_begin(),
                      e = instruction->idx_end();

    NodeIndex rhs = getTable().createNode(instruction);
    PointsToNode *gepNode = getTable().getValue(rhs);

    // get the id of the main pointer operand in the getelementptr
    id = getTable().getNode(instruction->getOperand(0));
    gepNode->use_push_back(id);

    for (; i != e; ++i) {
        Use *u = i;

        id = getTable().getOrCreateObjectNode(u->get());
        gepNode->use_push_back(id);
    }

    NodeIndex lhs = getTable().createPointerNode(instruction);
    addConstraint(lhs, rhs, ConstraintType::kAddressOf);
}

void ConstraintBuilder::processCallInstruction(CallInst *instruction) {
    NodeIndex id;

    User::op_iterator i = instruction->arg_begin(),
                      e = instruction->arg_end();

    NodeIndex rhs = getTable().createNode(instruction);
    PointsToNode *callNode = getTable().getValue(rhs);

    // get the id of the main pointer operand in the getelementptr
    id = getTable().getNode(instruction->getOperand(0));
    callNode->use_push_back(id);

    for (; i != e; ++i) {
        Use *u = i;

        id = getTable().getOrCreateObjectNode(u->get());
        callNode->use_push_back(id);
    }

    NodeIndex lhs = getTable().createPointerNode(instruction);
    addConstraint(lhs, rhs, ConstraintType::kCopy);
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
            NodeIndex rhs = getTable().createObjectNode(instruction);
            NodeIndex lhs = getTable().createPointerNode(instruction);
            addConstraint(lhs, rhs, ConstraintType::kAddressOf);
            return true;
        }

        case Instruction::Call:
        case Instruction::Invoke:
        {
            processCallInstruction(cast<CallInst>(instruction));
            return true;
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

            NodeIndex lhs = getTable().getOrCreatePointerNode(instruction);
            NodeIndex rhs = getTable().getPointerNode(instruction->getOperand(0));
            addConstraint(lhs, rhs, ConstraintType::kLoad);
            return true;
        }

        case Instruction::Store:
        {
            // example, store i32* %a, i32** %b
            if (!instruction->getOperand(0)->getType()->isPointerTy()) {
                return true;
            }

            NodeIndex lhs = getTable().getPointerNode(instruction->getOperand(1));
            NodeIndex rhs = getTable().getNode(instruction->getOperand(0));
            addConstraint(lhs, rhs, ConstraintType::kStore);
            return true;
        }

        case Instruction::GetElementPtr:
        {
            // example, %a = getelementptr {i32,i32}, {i32,i32}* %b, i1 0, i32 1
            processGetElementPtrNode(cast<GetElementPtrInst>(instruction));
            return true;
        }

        case Instruction::PHI:
        {
            if (!instruction->getType()->isPointerTy())
                return true;

            auto phi = dyn_cast<PHINode>(instruction);

            NodeIndex lhs = getTable().getOrCreatePointerNode(instruction);

            for (unsigned i = 0, e = phi->getNumIncomingValues(); i != e;
                i++) {
                NodeIndex rhs = getTable().getNode(phi->getIncomingValue(i));
                addConstraint(lhs, rhs, ConstraintType::kCopy);
            }
            return true;
        }

        case Instruction::BitCast:
        {
            if (!instruction->getType()->isPointerTy()) {
                return true;
            }

            NodeIndex lhs = getTable().getOrCreatePointerNode(instruction);
            NodeIndex rhs = getTable().getNode(instruction->getOperand(0));
            addConstraint(lhs, rhs, ConstraintType::kCopy);
            return true;
        }

        case Instruction::Select:
        {
            if (!instruction->getType()->isPointerTy()) {
                return true;
            }

            auto select = cast<SelectInst>(instruction);
            NodeIndex t = getTable().getNode(select->getTrueValue());
            NodeIndex f = getTable().getNode(select->getFalseValue());

            NodeIndex rhs = getTable().createNode(instruction);
            PointsToNode *selectNode = getTable().getValue(rhs);
            selectNode->use_push_back(t);
            selectNode->use_push_back(f);
            NodeIndex lhs = getTable().getOrCreatePointerNode(instruction);
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
