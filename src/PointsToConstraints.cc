#include "PointsToConstraints.h"

namespace ptsto {

void PointsToConstraints::dump(llvm::raw_ostream &os) const {
    for (auto &constraint : constraints_) {
        constraint.dump(os);
    }
}

std::set<NodeIndex> PointsToConstraints::getPointees(NodeIndex node) const {
    std::set<NodeIndex> result;

    for (const auto &constraint : constraints_) {
        if (constraint.getType() == ConstraintType::kAddressOf
            && constraint.getLHSId() == node) {
            result.insert(constraint.getRHSId());
        }
    }

    return result;
}

}
