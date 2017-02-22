#ifndef CONSTRAINT_BUILDER_H_
#define CONSTRAINT_BUILDER_H_

#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>

#include "PointerSymbolTable.h"
#include "PointsToConstraints.h"

namespace ptsto {

// helps in building constraints from IR
class ConstraintBuilder {
public:
    ConstraintBuilder() = default;

    PointsToConstraints &getConstraints();

    // get constraints for an LLVM instruction
    Constraint getConstraint(llvm::Instruction *instruction);

    PointerSymbolTable &getTable() {
        return *PointsToNode::table_;
    }
private:
    Constraint processCallInstruction(llvm::CallInst *instruction);
    Constraint processGetElementPtrNode(llvm::GetElementPtrInst* instruction);

    Constraint generateFromInstruction(llvm::Instruction *instruction);

    void addConstraint(NodeIndex lhs, NodeIndex rhs, ConstraintType type);
    Constraint makeConstraint(NodeIndex lhs, NodeIndex rhs, ConstraintType type);

    // This will hold all the points-to constraints
    PointsToConstraints constraints_;
};

}

#endif
