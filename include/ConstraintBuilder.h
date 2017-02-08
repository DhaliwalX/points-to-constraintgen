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

    // generate constraints for an LLVM module
    //  returns true if successful else false
    bool generateForModule(llvm::Module *module);

    // generate constraints for an LLVM function
    bool generateForFunction(llvm::Function *function);

    // generate constraints for an LLVM basic block
    bool generateForBasicBlock(llvm::BasicBlock *basicblock);

    PointerSymbolTable &getTable() {
        return *PointsToNode::table_;
    }
private:
    void processGetElementPtrNode(llvm::GetElementPtrInst* instruction);

    bool generateFromInstruction(llvm::Instruction *instruction);

    void addConstraint(NodeIndex lhs, NodeIndex rhs, ConstraintType type);

    // This will hold all the points-to constraints
    PointsToConstraints constraints_;
};

}

#endif
