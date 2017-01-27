#ifndef POINTER_NODE_H_
#define POINTER_NODE_H_

#include "Constraint.h"

#include <llvm/IR/Value.h>

namespace ptsto {

// this node represents a pointer or pointee
class PointsToNode {
public:
    enum class Type {
        kPointer,
        kPointee
    };

    PointsToNode(llvm::Value *value, Type type) : value_{ value }, type_{ type }
    { }

    llvm::Value *getValue() {
        return value_;
    }

    Type getType() {
        return type_;
    }

private:
    // pointer to llvm value node
    llvm::Value *value_;
    Type type_;
};

}

#endif
