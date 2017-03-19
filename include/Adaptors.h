/**
 * \file Adaptors.h
 * Special adaptors for making life a bit easier
 *
 * Iterating over llvm attributes of a llvm::Value is bit of
 * pain so it makes easier for iterating over those attributes
 * attributes like function arguments, users etc.
 *
 * So now you can write loops like
 *
 *     for (auto &arg : FunctionArguments(function))
 *         arg->dump();
 *
 */
#ifndef ADAPTORS_H_
#define ADAPTORS_H_

#include <llvm/IR/Function.h>

namespace ptsto {

/**
 * adaptor for iterating over arguments of llvm::function
 */
class FunctionArguments {
public:
    /**
     * makes easier for standard algorithms to adapt this class
     */
    using iterator = llvm::Function::arg_iterator;

    /**
     * \param function An llvm::Function variable
     */
    explicit FunctionArguments(llvm::Function &function)
        : function_{ function }
    { }

    /**
     * returns the iterator to the first argument
     * \return iterator to the first llvm::Argument
     */
    iterator begin() {
        return function_.arg_begin();
    }

    /**
     * returns the iterator to the last argument
     * \return iterator to the last llvm::Argument
     */
    iterator end() {
        return function_.arg_end();
    }

private:
    llvm::Function &function_;
};

}

#endif
