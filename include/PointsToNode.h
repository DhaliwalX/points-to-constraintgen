/**
 * \file PointsToNode.h
 *
 * Defines the PointsToNode class
 */
#ifndef POINTER_NODE_H_
#define POINTER_NODE_H_

#include <llvm/IR/Value.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Type.h>
#include <llvm/Support/raw_ostream.h>

namespace ptsto {
class PointerSymbolTable;

/**
 * This class represents the data type of id
 */
using NodeIndex = unsigned;

/**
 * Used for representing the invalid index, in case of errors
 */
static const int kInvalidIndex = std::numeric_limits<int>::max();

/**
 * A data structure to hold the information about a single pointer
 *
 * It tries to pack as much information as it can. You don't have to
 * work with LLVM Value when using this class. If you are using, then
 * please open an issue on GitHub.
 */
class PointsToNode {
public:
    /**
     * iterator definitions
     */
    using use_iterator = std::vector<NodeIndex>::iterator;
    using const_use_iterator = std::vector<NodeIndex>::const_iterator;
    using size_type = std::vector<NodeIndex>::size_type;

    /**
     * \todo Remove this table from here. Add it to a context class.
     */
    static PointerSymbolTable *table_;

    /**
     * Constructor
     *
     * \param value Pointer to the llvm::Value node, can be null in case of dummy
     * \param type Type of this node as above
     * \param id Id of the pointer
     */
    PointsToNode(llvm::Value *value, llvm::Type *type, NodeIndex id)
        : value_{ value }, type_{ type }, id_{ id }
    { }

    /**
     * Get the underlying llvm::Value. Returns null in case of dummy node
     *
     * \return pointer to underlying llvm::Value, can be null
     */
    llvm::Value *getValue() const {
        return value_;
    }

    /**
     * Get the type of this node, pointer or non-pointer
     *
     * \return type of this node, IMPORTANT: this is different from llvm::Type
     */
    llvm::Type *getType() {
        return type_;
    }

    // *
    //  * Set the type of this node
    //  *
    //  * \param t one of enum PointsToNode::Type
     
    // void setType(Type t) {
    //     type_ = t;
    // }

    /**
     * \return true if this node is a struct or field of a struct
     */
    bool hasStructTy() const {
        return type_->isStructTy();
    }

    /**
     * \return true if this node is an array or a member of an array
     */
    bool hasArrayTy() const {
        return type_->isArrayTy();
    }

    /**
     * \return true if this node represents an llvm::Value which has pointer type.
     */
    bool hasPointerTy() const {
        return type_->isPointerTy();
    }

    /**
     * \return llvm::Type* type of the underlying llvm::Value
     */
    llvm::Type *getLLVMType() {
        return type_;
    }

    /**
     * \return id of this node
     */
    NodeIndex getId() const {
        return id_;
    }

    /**
     * TODO: Remove this function, as it forces us to use single thread
     *
     * \param id id of the node which you want
     * \return reference to the PointsToNode object which has id `id`
     */
    static PointsToNode& GetNode(NodeIndex id);

    /**
     * These methods allow us to iterate over fields of a struct. Each field
     * of a struct is stored as a single use.
     *
     * \todo Point the user to an example of usage of these methods
     */


    /**
     * \return a use_iterator object that points to the beginning of use list
     */
    use_iterator use_begin() {
        return uses_.begin();
    }

    /**
     * \return a use_iterator object that points to the beginning of use list
     */
    const_use_iterator use_begin() const {
        return uses_.begin();
    }

    /**
     * \return a use_iterator object that points to the end of use list
     */
    use_iterator use_end() {
        return uses_.end();
    }

    /**
     * \return a use_iterator object that points to the end of use list
     */
    const_use_iterator use_end() const {
        return uses_.end();
    }

    /**
     * \return true if use list is empty
     */
    bool use_empty() const {
        return uses_.empty();
    }

    /**
     * \return the size of the use list
     */
    size_type use_size() const {
        return uses_.size();
    }

    /**
     * returns the nth use
     */
    NodeIndex getUse(unsigned n) {
        return uses_.at(n);
    }

    /**
     * Add a new use at the end of the use list
     * 
     * \param id id of the node
     */
    void use_push_back(NodeIndex id) {
        uses_.push_back(id);
    }

    /**
     * \return true if this node is a dummy node, means it does not have any
     *      llvm::Value* node
     */
    bool isDummy() const {
        return value_ == nullptr;
    }

    /**
     * dump this node to the os
     */
    void dump(llvm::raw_ostream &os) const;
protected:
    // pointer to llvm value node
    llvm::Value *value_;
    llvm::Type *type_;

    // what else values does this node uses. non empty in case of getelementptr
    std::vector<NodeIndex> uses_;
    NodeIndex id_;
};

}

#endif
