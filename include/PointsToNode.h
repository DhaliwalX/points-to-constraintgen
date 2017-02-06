#ifndef POINTER_NODE_H_
#define POINTER_NODE_H_

#include <llvm/IR/Value.h>
#include <llvm/IR/Type.h>

namespace ptsto {
class PointerSymbolTable;

using NodeIndex = unsigned;

static const int kInvalidIndex = std::numeric_limits<int>::max();

// this node represents a pointer or pointee
class PointsToNode {
public:
    static PointerSymbolTable *table_;
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

    bool hasStructTy() const {
        return value_->getType()->isStructTy();
    }

    bool hasArrayTy() const {
        return value_->getType()->isArrayTy();
    }

    bool hasPointerTy() const {
        return value_->getType()->isPointerTy();
    }

    llvm::Type *getLLVMType() {
        return value_->getType();
    }

    NodeIndex getId() const {
        return id_;
    }

protected:
    // pointer to llvm value node
    llvm::Value *value_;
    Type type_;
    NodeIndex id_;
};

}

#endif
