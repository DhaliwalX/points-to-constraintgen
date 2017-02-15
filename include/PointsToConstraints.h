#ifndef POINTS_TO_CONSTRAINTS
#define POINTS_TO_CONSTRAINTS

#include "Constraint.h"

#include <vector>
#include <set>

namespace ptsto {

class PointsToConstraints {
public:
    using iterator = std::vector<Constraint>::iterator;
    using const_iterator = std::vector<Constraint>::const_iterator;

    PointsToConstraints() = default;

    void insert(NodeIndex src, NodeIndex dest, ConstraintType type) {
        constraints_.push_back(Constraint(src, dest, type));
    }

    void insert(NodeIndex src, NodeIndex dest, ConstraintType type,
        llvm::Value *val) {
        auto idx = constraints_.size();
        insert(src, dest, type);
        irIndex_.insert(val, idx);
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

    // returns the vector of pointees of idx
    std::set<NodeIndex> getPointees(NodeIndex idx) const;

    // get constraint for particular instruction i
    // returns nullptr if there is no constraint for this instruction
    Constraint *get(llvm::Value *i) {
        auto it = irIndex_.find(i);
        if (it == irIndex_.end()) {
            return nullptr;
        }

        return &constraints_[it->second];
    }
private:
    std::vector<Constraint> constraints_;

    // to allow querying using llvm instruction
    std::map<llvm::Value*, int> irIndex_;
};

}

#endif
