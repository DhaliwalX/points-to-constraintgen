#ifndef CONSTRAINT_H_
#define CONSTRAINT_H_

#include <limits>

namespace ptsto {

using NodeIndex = unsigned;

static const int kInvalidIndex = std::numeric_limits<int>::max();

// PointsToConstraintType ::= the various types of assignments that can be 
//  present in the IR and correspond a class of constraints
enum class ConstraintType {
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
    Constraint(NodeIndex source, NodeIndex dest, ConstraintType type)
        : type_{ type }, source_{ source }, dest_{ dest }
    { }

    NodeIndex getDestination() {
        return dest_;
    }

    NodeIndex getSource() {
        return source_;
    }

    ConstraintType getType() {
        return type_;
    }

    void dump();
private:
    ConstraintType type_;
    NodeIndex source_;
    NodeIndex dest_;
};

}

#endif
