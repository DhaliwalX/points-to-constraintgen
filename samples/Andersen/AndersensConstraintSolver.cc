#include "AndersensConstraintSolver.h"

namespace ptsto {

// AndersonsPointeeSet 
// ===========================
//
void AndersonsPointeeSet::addPointee(NodeIndex pointee) {
    insert(pointee);
}

void AndersonsPointeeSet::merge(AndersonsPointeeSet *rhs) {
    mergeSet(rhs);
}


// AndersonsConstraintSolver
// ===========================
//
void AndersonsConstraintSolver::solveConstraints() {
    for (const auto &constraint : constraints_) {
        switch (constraint.getType()) {
            case ConstraintType::kAddressOf:
            {
                solveAddressOfEdge(constraint);
                break;
            }

            case ConstraintType::kCopy:
            {
                solveCopyEdge(constraint);
                break;
            }

            case ConstraintType::kStore:
            {
                solveStoreEdge(constraint);
                break;
            }

            case ConstraintType::kLoad:
            {
                solveLoadEdge(constraint);
                break;
            }
        }
    }
}

// a = &b
void
AndersonsConstraintSolver::solveAddressOfEdge(const Constraint &constraint) {
    NodeIndex src = constraint.getSource();
    NodeIndex dest = constraint.getDestination();

    graph_[src].addPointee(dest);
}

void
AndersonsConstraintSolver::mergeSet(NodeIndex src, NodeIndex dest) {
    graph_[src].mergeSet(&graph_[dest]);
}

// a = b
void
AndersonsConstraintSolver::solveCopyEdge(const Constraint &constraint) {
    NodeIndex src = constraint.getSource();
    NodeIndex dest = constraint.getDestination();

    mergeSet(src, dest);
}

// *a = b
void
AndersonsConstraintSolver::solveStoreEdge(const Constraint &constraint) {
    NodeIndex src = constraint.getSource();
    NodeIndex dest = constraint.getDestination();

    AndersonsPointeeSet &srcPointees = graph_[src];

    AndersonsPointeeSet::iterator it = srcPointees.begin();
    AndersonsPointeeSet::iterator end = srcPointees.end();
    for (; it != end; ++it) {
        mergeSet(*it, dest);
    }
}

// a = *b
void
AndersonsConstraintSolver::solveLoadEdge(const Constraint &constraint) {
    NodeIndex src = constraint.getSource();
    NodeIndex dest = constraint.getDestination();

    AndersonsPointeeSet &destPointees = graph_[dest];

    AndersonsPointeeSet::iterator it = destPointees.begin();
    AndersonsPointeeSet::iterator end = destPointees.end();

    for (; it != end; ++it) {
        mergeSet(src, *it);
    }
}


}
