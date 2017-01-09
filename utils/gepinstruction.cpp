#include <llvm/Pass.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>

using namespace llvm;
namespace {

template <class T>
class UserAdaptor {
public:
    using iterator = typename T::op_iterator;

    UserAdaptor(T &inst) : inst_{ inst }
    { }

    iterator begin() {
        return inst_.idx_begin();
    }

    iterator end() {
        return inst_.idx_end();
    }

private:
    T &inst_;
};

class GepInstructionPass : public FunctionPass {
public:
    static char ID;

    GepInstructionPass() : FunctionPass(ID)
    { }

    bool runOnFunction(Function &function) override {
        for (auto &bb : function) {
            for (auto &ir : bb) {
                auto instruction = dyn_cast<GetElementPtrInst>(&ir);

                if (instruction == nullptr)
                    continue;

                instruction->dump();
                instruction->getSourceElementType()->dump();
                instruction->getResultElementType()->dump();
                instruction->getType()->dump();

                errs() << "getNumIndices() = " << instruction->getNumIndices();
                errs() << "\nIndices: \n";

                for (auto &user : UserAdaptor<GetElementPtrInst>(*instruction)) {
                    user->dump();
                }
                errs() << "\n\n";
            }
        }
    }
};

}

char GepInstructionPass::ID = 0;

RegisterPass<GepInstructionPass> X("gep", "dumps information about gep instructions", false, false);
