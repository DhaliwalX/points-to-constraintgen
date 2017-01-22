#ifndef CONSTRAINT_H_
#define CONSTRAINT_H_


#include "Variable.h"

#include <llvm/IR/Instruction.h>
#include <llvm/Support/raw_ostream.h>

namespace pass {

class PointerInformation;

class Constraint {
public:
    Constraint(Variable *lhs, Variable *rhs, llvm::Instruction *instruction)
    : lhs_{ lhs }, rhs_{ rhs }, instruction_{ instruction }
    {
        processConstraintInformation();
    }

    ~Constraint() {
        delete lhs_;
        delete rhs_;
        // instruction_ is owned by llvm
    }

    Variable *&lhs() { return lhs_; }
    Variable *&rhs() { return rhs_; }

    llvm::Instruction *&instruction() { return instruction_; }

    void dump() {
        llvm::errs() << "\e[1m";
        lhs_->dump();
        llvm::errs() << " = ";
        rhs_->dump();
        llvm::errs() << "\e[0m";
        llvm::errs() << "\n";
    }


    void processConstraintInformation();
private:
    void processRHS();
    void processLHS();
    void processNode(ptrdiff_t &offset,
                    ptrdiff_t &pointer_arithematic,
                    ptrdiff_t &actual_offset,
                    PointerType &type,
                    std::string &name,
                    bool &offset_calculated,
                    PointerInfo *info,
                    bool lhs);

    Variable *lhs_;
    Variable *rhs_;

    llvm::Instruction *instruction_;
};

}

#endif
