#include "llvm/Pass.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"

#include "ConstraintBuilder.h"
#include "DotGraph.h"

#include <vector>
#include <utility>
#include <map>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>


using namespace llvm;
using namespace ptsto;

namespace {

struct ConstraintGen : public ModulePass {
    static char ID;

    ConstraintGen() : ModulePass(ID) { }

    bool runOnModule(Module &module) override {
        ConstraintBuilder builder;
        builder.generateForModule(&module);
        PointsToConstraints constraints = builder.getConstraints();

        constraints.dump();
        
        // generate dot file of the points-to graph
        DotGraphWriter graph;
        auto result = graph.write(constraints);

        auto fd = open("constraints.dot", O_CREAT | O_WRONLY, 0660);
        if (fd == -1) {
            errs() << "Unable to open constraint.dot: " << strerror(errno);
            return false;
        }
        raw_fd_ostream stream(fd, true);
        stream << result;
        stream.flush();

        return false;
    }
};

}

char ConstraintGen::ID = 0;
static RegisterPass<ConstraintGen> X("constraints", "ConstraintGen Pass",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);
