#include "Constraint.h"

#include <llvm/Support/raw_ostream.h>
namespace ptsto {

const char *typeToString[] = {
    "ADDRESS_OF",
    "LOAD",
    "STORE",
    "COPY"
};

void printVar(NodeIndex idx) {
    llvm::errs().changeColor(llvm::raw_ostream::WHITE, true);
    llvm::errs() << "a" << idx;
    llvm::errs().resetColor();
}

void Constraint::dump() const {

    llvm::errs().indent(4);
    switch (type_) {
        case ConstraintType::kAddressOf:
            printVar(source_);
            llvm::errs() << " = ";
            llvm::errs().changeColor(llvm::raw_ostream::GREEN, true) << "& ";
            printVar(dest_);
            break;

        case ConstraintType::kStore:
            llvm::errs().changeColor(llvm::raw_ostream::GREEN, true) << "* ";
            printVar(source_);
            llvm::errs() << " = ";
            printVar(dest_);
            break;

        case ConstraintType::kLoad:
            printVar(source_);
            llvm::errs() << " = ";
            llvm::errs().changeColor(llvm::raw_ostream::GREEN, true) << "* ";
            printVar(dest_);
            break;

        case ConstraintType::kCopy:
            printVar(source_);
            llvm::errs() << " = ";
            printVar(dest_);
            break;
    }

    llvm::errs() << "\n";
}

}
