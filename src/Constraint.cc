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

void printVar(PointsToNode *node, llvm::raw_ostream &os) {
    node->dump(os);
}

PointsToNode *Constraint::getLHSNode() {
    return PointsToNode::table_->getValue(dest_);
}

PointsToNode *Constraint::getLHSNode() const {
    return PointsToNode::table_->getValue(dest_);
}

PointsToNode *Constraint::getRHSNode() {
    return PointsToNode::table_->getValue(source_);
}

PointsToNode *Constraint::getRHSNode() const {
    return PointsToNode::table_->getValue(source_);
}

void Constraint::dump(llvm::raw_ostream &os) const {

    llvm::errs().indent(4);
    switch (type_) {
        case ConstraintType::kAddressOf:
            printVar(getLHSNode(), os);
            os << " = & ";
            printVar(getRHSNode(), os);
            break;

        case ConstraintType::kStore:
            os << "* ";
            printVar(getLHSNode(), os);
            os << " = ";
            printVar(getRHSNode(), os);
            break;

        case ConstraintType::kLoad:
            printVar(getLHSNode(), os);
            os << " = * ";
            printVar(getRHSNode(), os);
            break;

        case ConstraintType::kCopy:
            printVar(getLHSNode(), os);
            os << " = ";
            printVar(getRHSNode(), os);
            break;
    }

    os << "\n";
}

}
