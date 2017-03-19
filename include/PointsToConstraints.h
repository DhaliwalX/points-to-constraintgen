/**
 * \file PointsToConstraints.h
 *
 * This class is not as useful and will be removed
 */
#ifndef POINTS_TO_CONSTRAINTS
#define POINTS_TO_CONSTRAINTS

#include "Constraint.h"

#include <vector>
#include <set>

namespace ptsto {

static const Constraint kInvalidConstraint = Constraint(0, 0, ConstraintType::kInvalid);

class PointsToConstraints {
public:
    using iterator = std::vector<Constraint>::iterator;
    using const_iterator = std::vector<Constraint>::const_iterator;

    PointsToConstraints() = default;

    void insert(NodeIndex src, NodeIndex dest, ConstraintType type) {
        constraints_.push_back(Constraint(src, dest, type));
    }

    void push_back(Constraint c) {
        constraints_.push_back(c);
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

    void dump(llvm::raw_ostream &os) const;

    // returns the vector of pointees of idx
    std::set<NodeIndex> getPointees(NodeIndex idx) const;

private:
    std::vector<Constraint> constraints_;
};

}

#endif
