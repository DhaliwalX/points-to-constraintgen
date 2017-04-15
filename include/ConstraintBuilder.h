/**
 * \file ConstraintBuilder.h
 * Declares ConstraintBuilder class
 */
#ifndef CONSTRAINT_BUILDER_H_
#define CONSTRAINT_BUILDER_H_

#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>

#include "PointerSymbolTable.h"
#include "PointsToConstraints.h"

namespace ptsto {

/**
 * Main class for generating constraints for LLVM IR
 *
 * A simple use case of this class
 *
 *     ConstraintBuilder builder;
 *     // some stuff here...
 *     Constraint constraint = builder.getConstraint(instr);
 *     if (constraint != kInvalidConstraint) {
 *         // do whatever with constraint...
 *     }
 */
class ConstraintBuilder {
public:

    /**
     * Nothing much needed. So using defualt constructor.
     * \todo currently PointerSymbolTable is a static object. Store instead it in
     *      a context like object and pass that into ConstraintBuilder class
     */
    ConstraintBuilder() = default;

    /**
     * \return reference to the PointsToConstraints object
     */
    PointsToConstraints &getConstraints();

    /**
     * \return Single Constraint object for single LLVM IR statement
     */
    Constraint getConstraint(llvm::Instruction *instruction);

    /**
     * \return Generates ID for an Argument
     */
    NodeIndex generateIdForArgument(llvm::Argument *arg);

    /**
     * \return PointerSymbolTable
     */
    PointerSymbolTable &getTable() {
        return *PointsToNode::table_;
    }
private:

    /**
     * Method for processing call/invoke instructions
     *
     * \param instruction pointer to llvm::CallInst object
     * \return a Constraint object
     */
    Constraint processCallInstruction(llvm::CallInst *instruction);

    /**
     * Method for processing getelementptr instructions
     *
     * \param instruction pointer to llvm::GetElementPtrInst object
     * \return a Constraint object
     */
    Constraint processGetElementPtrNode(llvm::GetElementPtrInst* instruction);

    /**
     * More general method for generating constraint from an instruction
     *
     * \param instruction pointer to llvm::Instruction object
     * \return a Constraint object
     */
    Constraint generateFromInstruction(llvm::Instruction *instruction);

    /// TODO: remove this method as it is longer used
    void addConstraint(NodeIndex lhs, NodeIndex rhs, ConstraintType type);

    /**
     * creates and returns a Constraint object
     *
     * \param lhs id of lhs node
     * \param rhs id of rhs node
     * \param type type of the Constraint
     * \return a Constraint object
     */
    Constraint makeConstraint(NodeIndex lhs, NodeIndex rhs, ConstraintType type);

    // This will hold all the points-to constraints
    PointsToConstraints constraints_;
};

}

#endif
