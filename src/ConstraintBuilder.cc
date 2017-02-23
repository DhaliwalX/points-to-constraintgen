#include "ConstraintBuilder.h"
#include "Adaptors.h"

#include <llvm/IR/Instructions.h>
#include <llvm/Support/raw_ostream.h>

using namespace llvm;

namespace ptsto {

PointsToConstraints &ConstraintBuilder::getConstraints() {
    return constraints_;
}

Constraint ConstraintBuilder::makeConstraint(NodeIndex lhs, NodeIndex rhs,
                                            ConstraintType type) {
    return Constraint(lhs, rhs, type);
}

Constraint ConstraintBuilder::getConstraint(llvm::Instruction *instruction) {
    return generateFromInstruction(instruction);
}

Constraint ConstraintBuilder::processGetElementPtrNode(GetElementPtrInst *instruction)
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
    return makeConstraint(lhs, rhs, ConstraintType::kAddressOf);
}

Constraint ConstraintBuilder::processCallInstruction(CallInst *instruction) {
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
    return makeConstraint(lhs, rhs, ConstraintType::kCopy);
}

Constraint ConstraintBuilder::generateFromInstruction(Instruction *instruction) {
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
            return makeConstraint(lhs, rhs, ConstraintType::kAddressOf);
        }

        case Instruction::Call:
        case Instruction::Invoke:
        {
            return processCallInstruction(cast<CallInst>(instruction));
        }

        case Instruction::Ret:
        {
            //TODO
            errs() << "instruction not implemented: ";
            instruction->dump();
            return kInvalidConstraint;
        }


        case Instruction::Load:
        {
            // load instruction loads the value from an address
            // we only look for those load instruction whose resulting type
            // is a pointer type.
            //   example, %a = load i32*, i32** %b
            if (!instruction->getType()->isPointerTy()) {
                return kInvalidConstraint;
            }

            NodeIndex lhs = getTable().getOrCreatePointerNode(instruction);
            NodeIndex rhs = getTable().getPointerNode(instruction->getOperand(0));
            return makeConstraint(lhs, rhs, ConstraintType::kLoad);
        }

        case Instruction::Store:
        {
            // example, store i32* %a, i32** %b
            if (!instruction->getOperand(0)->getType()->isPointerTy()) {
                return kInvalidConstraint;
            }

            NodeIndex lhs = getTable().getPointerNode(instruction->getOperand(1));
            NodeIndex rhs = getTable().getNode(instruction->getOperand(0));
            return makeConstraint(lhs, rhs, ConstraintType::kStore);
        }

        case Instruction::GetElementPtr:
        {
            // example, %a = getelementptr {i32,i32}, {i32,i32}* %b, i1 0, i32 1
            return processGetElementPtrNode(cast<GetElementPtrInst>(instruction));
        }

        case Instruction::PHI:
        {
            if (!instruction->getType()->isPointerTy())
                return kInvalidConstraint;

            auto phi = dyn_cast<PHINode>(instruction);

            NodeIndex lhs = getTable().getOrCreatePointerNode(instruction);
            NodeIndex rhs = getTable().createNode(instruction);

            PointsToNode *rhsNode = getTable().getValue(rhs);

            for (unsigned i = 0, e = phi->getNumIncomingValues(); i != e;
                i++) {
                NodeIndex rhsUse = getTable().getNode(phi->getIncomingValue(i));
                rhsNode->use_push_back(rhsUse);
            }
            return makeConstraint(lhs, rhs, ConstraintType::kCopy);
        }

        case Instruction::BitCast:
        {
            if (!instruction->getType()->isPointerTy()) {
                return kInvalidConstraint;
            }

            NodeIndex lhs = getTable().getOrCreatePointerNode(instruction);
            NodeIndex rhs = getTable().getNode(instruction->getOperand(0));
            return makeConstraint(lhs, rhs, ConstraintType::kCopy);
        }

        case Instruction::Select:
        {
            if (!instruction->getType()->isPointerTy()) {
                return kInvalidConstraint;
            }

            auto select = cast<SelectInst>(instruction);
            NodeIndex t = getTable().getNode(select->getTrueValue());
            NodeIndex f = getTable().getNode(select->getFalseValue());

            NodeIndex rhs = getTable().createNode(instruction);
            PointsToNode *selectNode = getTable().getValue(rhs);
            selectNode->use_push_back(t);
            selectNode->use_push_back(f);
            NodeIndex lhs = getTable().getOrCreatePointerNode(instruction);
            return makeConstraint(lhs, rhs, ConstraintType::kCopy);
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
            return kInvalidConstraint;
        }


    }
}

}
