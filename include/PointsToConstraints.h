#ifndef POINTS_TO_CONSTRAINTS
#define POINTS_TO_CONSTRAINTS

#include "Constraint.h"

#include <vector>

namespace ptsto {

class PointsToConstraints {
public:
    using iterator = std::vector<Constraint>::iterator;
    using const_iterator = std::vector<Constraint>::const_iterator;

    PointsToConstraints() = default;

    iterator insert(const_iterator pos, Constraint &&constraint) {
        return constraints_.insert(pos, constraint);
    }

    template <typename... Args>
    void emplace_back(Args&&... args) {
        constraints_.emplace_back(std::forward<Args>(args)...);
    }

    iterator begin() {
        return constraints_.begin();
    }

    iterator end() {
        return constraints_.end();
    }

    void dump();
private:
    std::vector<Constraint> constraints_;
};

}

#endif
