/**
 * \file PointerSymbolTable.h
 *
 * Defines PointerSymbolTable. A data structure for storing various mappings
 */
#ifndef POINTER_SYMBOL_TABLE_H_
#define POINTER_SYMBOL_TABLE_H_

#include "PointsToNode.h"

#include <map>
#include <vector>

namespace ptsto {

/**
 * This data structure stores the PointsToNode objects in an array and to create
 * a mapping between PointsToNode object and llvm::Value object it assigns the
 * index of array to the llvm::Value. Normal Objects and pointers are stored
 * separately.
 *
 * This class is used internally in the library.
 */
class PointerSymbolTable {
public:
    /**
     * Useful method for getting PointsToNode object stored at a particular index
     * \param index index of the element
     * \return a pointer to the PointsToNode object stored in the array
     */
    PointsToNode *getValue(NodeIndex index);

    /**
     * creates a node representing a pointer and returns an index for future 
     * reference and easy retrieval
     *
     * \param val pointer to the llvm::Value object
     * \return id of the newly created node
     */
    NodeIndex createPointerNode(llvm::Value *val, llvm::Type *type);

    // *
    //  * creates a node representing a non-pointer llvm::Value and returns an index
    //  * for future reference
    //  *
    //  * \param val pointer to the llvm::Value object
    //  * \return id of the newly created node
     
    // NodeIndex createObjectNode(llvm::Value *val);

    /**
     * returns the index of previously created node corresponding
     * to the `val`. If there is no such node, then creates one
     * This node should refer to an llvm::Value that has pointer type.
     *
     * \param val pointer to the llvm::Value object
     * \return id of the newly created node
     */
    NodeIndex getOrCreatePointerNode(llvm::Value *val, llvm::Type* type);

    // /**
    //  * returns the index of previously created node corresponding
    //  * to the `val`. If there is no such node, then creates one
    //  * This node should refer to an llvm::Value that has non-pointer type.
    //  *
    //  * \param val pointer to the llvm::Value object
    //  * \return id of the old or newly created node
    //  */
    // NodeIndex getOrCreateObjectNode(llvm::Value *val);

    // /**
    //  * returns the index of previously created node corresponding to the `val`
    //  *
    //  * \param val pointer to the llvm::Value object which should have pointer type
    //  * \return id of the node
    //  */
    // NodeIndex getPointerNode(llvm::Value *val);

    // /**
    //  * returns the index of previously created node corresponding to the `val`
    //  *
    //  * \param val pointer to the llvm::Value object which should have non pointer type
    //  * \return id of the node
    //  */
    // NodeIndex getObjectNode(llvm::Value *val);

    // *
    //  * returns the index of the node with respect to the type
    //  *
    //  * \param val pointer to an llvm::Value object
    //  * \param type type of the node that is being expected
    //  * \return id of the node
     
    // NodeIndex getNode(llvm::Value *val, PointsToNode::Type type);

    // /**
    //  * More general method for searching node for an llvm::Value. It tries to locate
    //  * the node in the symbol table using type of val
    //  *
    //  * \param val pointer to an llvm::Value object
    //  * \return id of the node
    //  */
    // NodeIndex getNode(llvm::Value *val);

    // /**
    //  * creates a new node
    //  */
    // NodeIndex createNode(llvm::Value *val);

    /**
     * returns the size of the symbol table
     */
    std::vector<PointsToNode>::size_type size() {
        return nodes_.size();
    }

    /**
     * Dumps the complete symbol table to the output stream. Useful for debugging
     */
    void dump(llvm::raw_ostream &os) const;

    /**
     * creates a dummy node in the table
     */
    NodeIndex createDummyNode(llvm::AllocaInst *inst);

    /**
     * creates a dummy node for other instructions
     */
    NodeIndex createDummyNode(llvm::Instruction *inst);
private:
    NodeIndex getNode(std::map<llvm::Value*, NodeIndex> &map, llvm::Value *val);
    NodeIndex createNode(std::map<llvm::Value*, NodeIndex> &map,
                        llvm::Value *val,
                        llvm::Type *type);

    // keeps track of pointers
    std::map<llvm::Value*, NodeIndex> pointerMap_;

    std::vector<PointsToNode> nodes_;
};

}

#endif
