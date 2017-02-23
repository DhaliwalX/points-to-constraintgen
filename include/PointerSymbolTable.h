#ifndef POINTER_SYMBOL_TABLE_H_
#define POINTER_SYMBOL_TABLE_H_

#include "PointsToNode.h"

#include <map>
#include <vector>

namespace ptsto {

class PointerSymbolTable {
public:
    // returns the value at index, can be pointer or pointee
    PointsToNode *getValue(NodeIndex index);

    // creates a node representing a pointer and returns a index
    // for future reference and easy retrieval
    NodeIndex createPointerNode(llvm::Value *val);

    // creates a node representing an object
    NodeIndex createObjectNode(llvm::Value *val);

    // returns the index of previously created node corresponding
    // to the `val`. If there is no such node, then creates one
    NodeIndex getOrCreatePointerNode(llvm::Value *val);

    NodeIndex getOrCreateObjectNode(llvm::Value *val);

    NodeIndex getPointerNode(llvm::Value *val);

    NodeIndex getObjectNode(llvm::Value *val);

    // returns the index of the node with respect to the type
    NodeIndex getNode(llvm::Value *val, PointsToNode::Type type);

    NodeIndex getNode(llvm::Value *val);

    // creates a new node
    NodeIndex createNode(llvm::Value *val);

    // returns the size of the symbol table
    std::vector<PointsToNode>::size_type size() {
        return nodes_.size();
    }

    // dumps the symbol table
    void dump(llvm::raw_ostream &os) const;

private:
    NodeIndex getNode(std::map<llvm::Value*, NodeIndex> &map, llvm::Value *val);
    NodeIndex createNode(std::map<llvm::Value*, NodeIndex> &map,
                        llvm::Value *val,
                        PointsToNode::Type type);

    // keeps track of pointers
    std::map<llvm::Value*, NodeIndex> pointerMap_;

    // it keeps track of objects whose address is being used
    std::map<llvm::Value*, NodeIndex> objectMap_;

    std::vector<PointsToNode> nodes_;
};

}

#endif
