#include "PointsToNode.h"
#include "PointerSymbolTable.h"

namespace ptsto {

PointerSymbolTable *PointsToNode::table_ = new PointerSymbolTable();


void PointsToNode::dump(llvm::raw_ostream &os) const {
    os << getId();

    if (use_empty())
        return;

    os << " uses: { ";

    if (use_size() == 1) {
        os << *use_begin() << " }";
        return;
    }

    for (auto i = use_begin(), e = use_end() - 1; i != e; i++) {
        os << *i << ", ";
    }

    os << *(use_end() - 1) << " }";
}

}
