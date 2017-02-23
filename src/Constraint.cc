#include "Constraint.h"
#include "PointerSymbolTable.h"

#include <llvm/Support/raw_ostream.h>
namespace ptsto {

const char *typeToString[] = {
    "ADDRESS_OF",
    "LOAD",
    "STORE",
    "COPY"
};

void printVar(NodeIndex idx, llvm::raw_ostream &os) {
    os << "a" << idx;
}

PointsToNode *Constraint::getLHSNode() {
    return PointsToNode::table_->getValue(dest_);
}

PointsToNode *Constraint::getRHSNode() {
    return PointsToNode::table_->getValue(source_);
}

void Constraint::dump(llvm::raw_ostream &os) const {

    llvm::errs().indent(4);
    switch (type_) {
        case ConstraintType::kAddressOf:
            printVar(source_, os);
            os << " = & ";
            printVar(dest_, os);
            break;

        case ConstraintType::kStore:
            os << "* ";
            printVar(source_, os);
            os << " = ";
            printVar(dest_, os);
            break;

        case ConstraintType::kLoad:
            printVar(source_, os);
            os << " = * ";
            printVar(dest_, os);
            break;

        case ConstraintType::kCopy:
            printVar(source_, os);
            os << " = ";
            printVar(dest_, os);
            break;
    }

    os << "\n";
}

}
