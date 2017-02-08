#include "PointerSymbolTable.h"

namespace ptsto {

PointsToNode *PointerSymbolTable::getValue(NodeIndex index) {
    return &nodes_.at(index);
}

NodeIndex PointerSymbolTable::createPointerNode(llvm::Value *val) {
    return createNode(pointerMap_, val, PointsToNode::Type::kPointer);
}

NodeIndex PointerSymbolTable::createObjectNode(llvm::Value *val) {
    return createNode(objectMap_, val, PointsToNode::Type::kPointee);
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
    if (it != objectMap_.end()) {
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
    nodes_.emplace_back(val, type);

    assert(!map.count(val));
    map.insert({ val, index });
    return index;
}

NodeIndex PointerSymbolTable::createNode(llvm::Value *val) {
    auto index = nodes_.size();
    nodes_.push_back(PointsToNode(val, PointsToNode::Type::kPointee));
    return index;
}

}
