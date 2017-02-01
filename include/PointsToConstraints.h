#ifndef POINTS_TO_CONSTRAINTS
#define POINTS_TO_CONSTRAINTS

#include "Constraint.h"

#include <set>

namespace ptsto {

class PointsToConstraints {
public:
    using iterator = std::set<Constraint>::iterator;
    using const_iterator = std::set<Constraint>::const_iterator;

    PointsToConstraints() = default;

    void insert(NodeIndex src, NodeIndex dest, ConstraintType type) {
        constraints_.insert(Constraint(src, dest, type));
    }

    iterator begin() {
        return constraints_.begin();
    }

    const_iterator begin() const {
        return constraints_.cbegin();
    }

    iterator end() {
        return constraints_.end();
    }

    const_iterator end() const {
        return constraints_.cend();
    }

    void dump() const;

    // returns the set of pointees of idx
    std::set<NodeIndex> getPointees(NodeIndex idx) const;

private:
    std::set<Constraint> constraints_;
};

}

#endif
