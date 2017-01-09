#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"

#include <string>

using namespace llvm;

namespace {

struct AssignTemporaryName : public FunctionPass {
	static char ID;

	static size_t counter_;

	AssignTemporaryName() : FunctionPass(ID) { }

	std::string getTemporaryName() {
		counter_++;
		return std::to_string(counter_) + "tmp" + std::to_string(counter_);
	}

	bool runOnFunction(Function &function) override {
		for (auto &bb : function) {
			for (auto &instruction : bb) {
				if (instruction.getName().size() == 0) {
					instruction.dump();
					if (!instruction.getType()->isVoidTy())
						instruction.setName(getTemporaryName());
				}
			}
		}
		return true;
	}
};

size_t AssignTemporaryName::counter_ = 0;
char AssignTemporaryName::ID = 0;

RegisterPass<AssignTemporaryName> X("assign-temp",
	"assigns name to the unnamed values", true, true);

} // anonymous namespace 

