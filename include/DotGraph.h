#ifndef DOT_GRAPH_H_
#define DOT_GRAPH_H_

#include "PointsToConstraints.h"

#include <sstream>

namespace ptsto {

class DotGraphWriter {
    std::ostringstream str;
public:
    void writeEdge(NodeIndex src, NodeIndex dest) {
        str << "\t\"" << src << "\" -> \"" << dest << "\"\n";
    }

    std::string write(const PointsToConstraints &constraints) {
        str << "digraph G {\n";
        for (const auto &constraint : constraints) {

            switch (constraint.getType()) {
                case ConstraintType::kAddressOf:
                {
                    writeEdge(constraint.getSource(), constraint.getDestination());
                    break;
                }

                case ConstraintType::kStore:
                {
                    // get pointees of source node
                    auto pointees = constraints.getPointees(constraint.getSource());
                    auto pointeesOfDest = constraints.getPointees(constraint.getDestination());
                    for (const auto &pointee : pointees) {
                        for (const auto &pointeeOfDest : pointeesOfDest) {
                            writeEdge(pointee, pointeeOfDest);
                        }
                    }
                    break;
                }

                case ConstraintType::kLoad:
                {
                    auto pointees = constraints.getPointees(constraint.getDestination());

                    for (const auto &pointee : pointees) {
                        auto pointeesOfPointee = constraints.getPointees(pointee);

                        for (const auto &pointeeOfPointee : pointeesOfPointee) {
                            writeEdge(constraint.getSource(), pointeeOfPointee);
                        }
                    }
                    break;
                }

                case ConstraintType::kCopy:
                {
                    auto pointees = constraints.getPointees(constraint.getDestination());

                    for (const auto &pointee : pointees) {
                        writeEdge(constraint.getSource(), pointee);
                    }
                    break;
                }
            }
        }
        str << "}\n";
        return str.str();
    }
};

}


#endif
