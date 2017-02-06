#include "PointsToNode.h"
#include "PointerSymbolTable.h"

namespace ptsto {

PointerSymbolTable *PointsToNode::table_ = new PointerSymbolTable();

}
