#ifndef ADAPTORS_H_
#define ADAPTORS_H_

#include <llvm/IR/Function.h>

namespace ptsto {

class FunctionArguments {
public:
    using iterator = llvm::Function::arg_iterator;

    FunctionArguments(llvm::Function &function)
        : function_{ function }
    { }

    iterator begin() {
        return function_.arg_begin();
    }

    iterator end() {
        return function_.arg_end();
    }

private:
    llvm::Function &function_;
};

}

#endif
