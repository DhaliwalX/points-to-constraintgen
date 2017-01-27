#include "PointsToConstraints.h"

namespace ptsto {

void PointsToConstraints::dump() {
    for (auto &constraint : constraints_) {
        constraint.dump();
    }
}

}
