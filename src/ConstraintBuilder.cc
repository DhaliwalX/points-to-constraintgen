#include "ConstraintBuilder.h"
#include "Adaptors.h"

#include <llvm/IR/Instructions.h>
#include <llvm/Support/raw_ostream.h>

using namespace llvm;

namespace ptsto {

Constraint ConstraintBuilder::makeConstraint(NodeIndex lhs, NodeIndex rhs,
                                            ConstraintType type) {
    return Constraint(lhs, rhs, type);
}

std::vector<Constraint> ConstraintBuilder::getConstraint(llvm::Instruction *instruction) {
    return generateFromInstruction(instruction);
}

NodeIndex ConstraintBuilder::generateIdForArgument(Argument *arg) {
    if (arg->getType()->isPointerTy())
        return getTable().getOrCreatePointerNode(arg);
    else
        return getTable().getOrCreateObjectNode(arg);
}

std::vector<Constraint>
    ConstraintBuilder::processGetElementPtrNode(GetElementPtrInst *instruction)
{
    std::vector<Constraint> ret;
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
    ret.push_back(makeConstraint(lhs, rhs, ConstraintType::kAddressOf));
    return ret;
}

std::vector<Constraint> ConstraintBuilder::processCallInstruction(CallInst *instruction) {
    NodeIndex id;

    // User::op_iterator i = instruction->arg_begin(),
    //                   e = instruction->arg_end();

    // NodeIndex rhs = getTable().createNode(instruction);
    // PointsToNode *callNode = getTable().getValue(rhs);

    // // get the id of the main pointer operand in the getelementptr
    // id = getTable().getNode(instruction->getOperand(0));
    // callNode->use_push_back(id);

    // for (; i != e; ++i) {
    //     Use *u = i;

    //     id = getTable().getOrCreateObjectNode(u->get());
    //     callNode->use_push_back(id);
    // }

    // NodeIndex lhs = getTable().createPointerNode(instruction);
    // return makeConstraint(lhs, rhs, ConstraintType::kCopy);
    return {};
}

std::vector<Constraint> ConstraintBuilder::generateFromInstruction(Instruction *instruction) {
    std::vector<Constraint> ret;
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
            NodeIndex rhs;
            rhs = getTable().createDummyNode(dyn_cast<AllocaInst>(instruction));
            NodeIndex lhs = getTable().createPointerNode(instruction);
            ret.push_back(makeConstraint(lhs, rhs, ConstraintType::kAddressOf));
            break;
        }

        case Instruction::Call:
        case Instruction::Invoke:
        {
            // not handling this case
            // return processCallInstruction(cast<CallInst>(instruction));
        }

        case Instruction::Ret:
        {
            // not handling, as this is more related to the analysis
            break;
        }


        case Instruction::Load:
        {
            // load instruction loads the value from an address
            // we only look for those load instruction whose resulting type
            // is a pointer type.
            //   example, %a = load i32*, i32** %b
            if (!instruction->getType()->isPointerTy()) {
                break;
            }

            NodeIndex lhs = getTable().getOrCreatePointerNode(instruction);
            NodeIndex rhs = getTable().getOrCreatePointerNode(instruction->getOperand(0));
            ret.push_back(makeConstraint(lhs, rhs, ConstraintType::kLoad));
            break;
        }

        case Instruction::Store:
        {
            // example, store i32* %a, i32** %b
            if (!instruction->getOperand(0)->getType()->isPointerTy()) {
                break;
            }

            NodeIndex lhs = getTable().getOrCreatePointerNode(instruction->getOperand(1));
            NodeIndex rhs = getTable().getOrCreatePointerNode(instruction->getOperand(0));
            ret.push_back(makeConstraint(lhs, rhs, ConstraintType::kStore));
            break;
        }

        case Instruction::GetElementPtr:
        {
            // example, %a = getelementptr {i32,i32}, {i32,i32}* %b, i1 0, i32 1
            return processGetElementPtrNode(cast<GetElementPtrInst>(instruction));
        }

        case Instruction::PHI:
        {
            if (!instruction->getType()->isPointerTy())
                break;

            auto phi = dyn_cast<PHINode>(instruction);

            NodeIndex lhs = getTable().getOrCreatePointerNode(instruction);

            for (unsigned i = 0, e = phi->getNumIncomingValues(); i != e;
                i++) {
                NodeIndex rhs = getTable().getOrCreatePointerNode(phi->getIncomingValue(i));
                ret.push_back(makeConstraint(lhs, rhs, ConstraintType::kCopy));
            }
            break;
        }

        case Instruction::BitCast:
        {
            if (!instruction->getType()->isPointerTy()) {
                break;
            }

            NodeIndex lhs = getTable().getOrCreatePointerNode(instruction);
            NodeIndex rhs = getTable().getOrCreatePointerNode(instruction->getOperand(0));
            ret.push_back(makeConstraint(lhs, rhs, ConstraintType::kCopy));
            break;
        }

        case Instruction::Select:
        {
            if (!instruction->getType()->isPointerTy()) {
                break;
            }

            auto select = cast<SelectInst>(instruction);
            NodeIndex t = getTable().getOrCreatePointerNode(select->getTrueValue());
            NodeIndex f = getTable().getOrCreatePointerNode(select->getFalseValue());
            NodeIndex lhs = getTable().getOrCreatePointerNode(instruction);
            ret.push_back(makeConstraint(lhs, t, ConstraintType::kCopy));
            ret.push_back(makeConstraint(lhs, f, ConstraintType::kCopy));
            break;
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
            break;
        }
    } // switch

    return ret;
}

}
