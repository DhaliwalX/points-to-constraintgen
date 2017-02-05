#ifndef ANDERSONS_CONSTRAINT_SOLVER_H_
#define ANDERSONS_CONSTRAINT_SOLVER_H_

#include "ADT/RecursiveSet.h"
#include "PointsToConstraints.h"
#include "PointerSymbolTable.h"

#include <vector>
#include <string>

namespace ptsto {

class AndersonsPointeeSet : public RecursiveSet<NodeIndex> {
public:
    using iterator = RecursiveSet<NodeIndex>::iterator;

    AndersonsPointeeSet()
        : RecursiveSet<NodeIndex>()
    { }

    AndersonsPointeeSet(const AndersonsPointeeSet&) = default;
    AndersonsPointeeSet &operator=(const AndersonsPointeeSet&) = default;

    void addPointee(NodeIndex pointee);

    void merge(AndersonsPointeeSet *rhs);
};

using AndersonsPointsToGraph = std::vector<AndersonsPointeeSet>;

class AndersonsConstraintSolver {
public:
    AndersonsConstraintSolver(PointsToConstraints &constraints,
                              PointerSymbolTable *table)
        : constraints_{ constraints }, table_{ table }, graph_{ table->size() }
    { }

    // solve the constraints
    void solveConstraints();

    // dump the dot file
    std::string dumpDOT();

    // returns the internal
    AndersonsPointsToGraph &getGraph() {
        return graph_;
    }
private:

    void solveAddressOfEdge(const Constraint &constraint);
    void solveLoadEdge(const Constraint &constraint);
    void solveStoreEdge(const Constraint &constraint);
    void solveCopyEdge(const Constraint &constraint);

    void mergeSet(NodeIndex src, NodeIndex dest);

    PointsToConstraints &constraints_;
    PointerSymbolTable *table_;
    AndersonsPointsToGraph graph_;
};

}

#endif
