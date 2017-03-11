#ifndef POINTER_NODE_H_
#define POINTER_NODE_H_

#include <llvm/IR/Value.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Type.h>
#include <llvm/Support/raw_ostream.h>

namespace ptsto {
class PointerSymbolTable;

using NodeIndex = unsigned;

static const int kInvalidIndex = std::numeric_limits<int>::max();

// this node represents a pointer or pointee
class PointsToNode {
    using use_iterator = std::vector<NodeIndex>::iterator;
    using const_use_iterator = std::vector<NodeIndex>::const_iterator;
    using size_type = std::vector<NodeIndex>::size_type;
public:
    static PointerSymbolTable *table_;
    enum class Type {
        kNonPointer,
        kPointer
    };

    PointsToNode(llvm::Value *value, Type type, NodeIndex id)
        : value_{ value }, type_{ type }, id_{ id }
    { }

    llvm::Value *getValue() const {
        return value_;
    }

    Type getType() {
        return type_;
    }

    void setType(Type t) {
        type_ = t;
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

    virtual bool isGEPNode() const {
        return false;
    }

    llvm::Type *getLLVMType() {
        return value_->getType();
    }

    NodeIndex getId() const {
        return id_;
    }

    void setId(NodeIndex id) {
        id_ = id;
    }

    static PointsToNode& GetNode(NodeIndex id);

    use_iterator use_begin() {
        return uses_.begin();
    }

    const_use_iterator use_begin() const {
        return uses_.begin();
    }

    use_iterator use_end() {
        return uses_.end();
    }

    const_use_iterator use_end() const {
        return uses_.end();
    }

    // returns true if use list is empty
    bool use_empty() const {
        return uses_.empty();
    }

    // returns the size of the use list
    size_type use_size() const {
        return uses_.size();
    }

    // returns the nth use
    NodeIndex getUse(unsigned n) {
        return uses_.at(n);
    }

    void use_push_back(NodeIndex id) {
        uses_.push_back(id);
    }

    // returns true if this node is a dummy node, means it does not have any
    // llvm::Value* node
    bool isDummy() const {
        return value_ == nullptr;
    }

    void dump(llvm::raw_ostream &os) const;
protected:
    // pointer to llvm value node
    llvm::Value *value_;
    bool is_gep_node_;
    Type type_;

    // what else values does this node uses. non empty in case of getelementptr
    std::vector<NodeIndex> uses_;
    NodeIndex id_;
};

}

#endif
