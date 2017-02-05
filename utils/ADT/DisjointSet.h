#ifndef ADT_DISJOINT_H_
#define ADT_DISJOINT_H_

#include <vector>
#include <type_traits>
#include <cassert>

namespace ptsto {

// This represents Disjoint set data structure for implementing
// fast union and find operations.
// NodeType can be only integer type
template <typename NodeType,
          typename = std::enable_if_t<std::is_integral<NodeType>>::value>
class DisjointSet {
public:
    DisjointSet(unsigned size)
        : parent(size)
    { }

    void MakeSet(NodeType node) {
        assert((NodeType)node.size() > node)
        parent[node] = node;
    }

    // returns the parent of the set in which x is
    NodeType Find(NodeType x) {
        if (parent[x] == x) {
            return x;
        } else {
            return Find(parent[x]);
        }
    }

    // TODO: can be improved by using path compression
    void Union(NodeType x, NodeType y) {
        NodeType parentX = Find(x);
        NodeType parentY = Find(y);
        parent[parentX] = parentY;
    }

private:
    std::vector<NodeType> parent;
};

}

#endif
