#include "AndersensConstraintSolver.h"

#include <sstream>

namespace ptsto {

class AndersonsDotGraphWriter {
public:
    AndersonsDotGraphWriter(std::ostream &os)
        : os_{os} { }

    void initGraph(std::string name);

    void emitEdge(NodeIndex src, NodeIndex dest);

    void emitGraph(AndersonsPointsToGraph &graph);

    void endGraph();

private:
    std::ostream &os_;
};


void AndersonsDotGraphWriter::initGraph(std::string name) {
    os_ << "digraph " << name << " {\n";
}

void AndersonsDotGraphWriter::emitEdge(NodeIndex src, NodeIndex dest) {
    os_ << "\t\"" << src << "\" -> \"" << dest << "\"\n";
}

void AndersonsDotGraphWriter::emitGraph(AndersonsPointsToGraph &graph) {
    initGraph("G");
    for (AndersonsPointsToGraph::size_type i = 0;
        i != graph.size(); i++) {
        auto &pointees = graph[i];

        // should we do that?
        if (pointees.empty()) continue;

        AndersonsPointeeSet::iterator it = pointees.begin();
        AndersonsPointeeSet::iterator end = pointees.end();

        for (; it != end; ++it) {
            emitEdge(i, *it);
        }
    }

    endGraph();
}

void AndersonsDotGraphWriter::endGraph() {
    os_ << "}\n";
}

std::string AndersonsConstraintSolver::dumpDOT() {
    std::ostringstream os;
    AndersonsDotGraphWriter writer(os);
    writer.emitGraph(graph_);
    return os.str();
}

}


