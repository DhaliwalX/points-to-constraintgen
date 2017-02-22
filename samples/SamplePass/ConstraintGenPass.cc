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
        return false;
    }
};

}

char ConstraintGen::ID = 0;
static RegisterPass<ConstraintGen> X("constraints", "ConstraintGen Pass",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);
