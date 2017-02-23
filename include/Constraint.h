#ifndef CONSTRAINT_H_
#define CONSTRAINT_H_

#include "PointsToNode.h"
#include <llvm/Support/raw_ostream.h>
#include <limits>

namespace ptsto {

class PointerSymbolTable;

// PointsToConstraintType ::= the various types of assignments that can be 
//  present in the IR and correspond a class of constraints
enum class ConstraintType {
    kInvalid,
    kAddressOf, /* e.g. %a = alloca %type, or in C, a = &b */
    kLoad,      /* e.g. %a = load %type*, %type** %b,  in C as `a = *b` */
    kStore,     /* e.g. store %type* %b, %type** %a,   in C as `*a = b` */
    kCopy       /* e.g. %a = getelementptr %type, %type* %b, in C as `a = b` */
};

// Constraint ::= holds the information about a constraint for 
//  points-to analysis. It stores the pointer to symbol table entry of source and
//  destination. It also stores the type of constraint which can be one of above.
class Constraint {
public:
    Constraint(NodeIndex dest, NodeIndex source, ConstraintType type)
        : type_{ type }, source_{ source }, dest_{ dest }
    { }

    NodeIndex getDestination() const {
        return dest_;
    }

    NodeIndex getSource() const {
        return source_;
    }

    ConstraintType getType() const {
        return type_;
    }

    void dump(llvm::raw_ostream &os) const;

    PointsToNode *getLHSNode();
    PointsToNode *getLHSNode() const;

    PointsToNode *getRHSNode();
    PointsToNode *getRHSNode() const;
private:
    ConstraintType type_;
    NodeIndex source_;
    NodeIndex dest_;
};

static inline bool operator<(const Constraint &lhs, const Constraint &rhs) {
    return lhs.getSource() < rhs.getSource()
                || lhs.getDestination() < rhs.getDestination();
}

static inline bool operator==(const Constraint &lhs, const Constraint &rhs) {
    return lhs.getSource() == rhs.getSource()
            && lhs.getDestination() == rhs.getDestination()
            && lhs.getType() == rhs.getType();
}

}

#endif
