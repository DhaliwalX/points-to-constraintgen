#include "PointerSymbolTable.h"

namespace ptsto {

PointsToNode *PointerSymbolTable::getValue(NodeIndex index) {
    return &nodes_.at(index);
}

NodeIndex PointerSymbolTable::createPointerNode(llvm::Value *val) {
    return createNode(pointerMap_, val, PointsToNode::Type::kPointer);
}

NodeIndex PointerSymbolTable::createObjectNode(llvm::Value *val) {
    if (val->getType()->isPointerTy())
        return createNode(objectMap_, val, PointsToNode::Type::kPointer);
    else
        return createNode(objectMap_, val, PointsToNode::Type::kNonPointer);
}

NodeIndex PointerSymbolTable::getOrCreatePointerNode(llvm::Value *val) {
    auto it = pointerMap_.find(val);
    if (it == pointerMap_.end()) {
        return createPointerNode(val);
    }

    return it->second;
}

NodeIndex PointerSymbolTable::getOrCreateObjectNode(llvm::Value *val) {
    auto it = objectMap_.find(val);
    if (it == objectMap_.end()) {
        return createObjectNode(val);
    }

    return it->second;
}

NodeIndex PointerSymbolTable::getPointerNode(llvm::Value *val) {
    return getNode(pointerMap_, val);
}

NodeIndex PointerSymbolTable::getObjectNode(llvm::Value *val) {
    return getNode(objectMap_, val);
}

NodeIndex PointerSymbolTable::getNode(llvm::Value *val, PointsToNode::Type type) {
    return type == PointsToNode::Type::kPointer
            ? getPointerNode(val)
            : getObjectNode(val);
}

NodeIndex PointerSymbolTable::getNode(llvm::Value *val) {
    auto index = getPointerNode(val);
    if (index == kInvalidIndex) {
        return getObjectNode(val);
    }

    return index;
}

NodeIndex PointerSymbolTable::getNode(std::map<llvm::Value*, NodeIndex> &map,
                                      llvm::Value *val) {
    auto it = map.find(val);

    if (it == map.end()) {
        return kInvalidIndex;
    }

    return it->second;
}

NodeIndex PointerSymbolTable::createNode(std::map<llvm::Value*, NodeIndex> &map,
                    llvm::Value *val,
                    PointsToNode::Type type) {
    auto index = nodes_.size();
    nodes_.emplace_back(val, type, index);
    assert(!map.count(val));
    map.insert({ val, index });
    return index;
}

NodeIndex PointerSymbolTable::createNode(llvm::Value *val) {
    auto index = nodes_.size();
    nodes_.push_back(PointsToNode(val, PointsToNode::Type::kPointer, index));
    return index;
}

void PointerSymbolTable::dump(llvm::raw_ostream &os) const {
    for (auto &node : nodes_) {
        os << node.getId() << ": ";
        node.getValue()->dump();
    }
}

}
