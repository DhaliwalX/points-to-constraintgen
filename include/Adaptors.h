#ifndef ADAPTORS_H_
#define ADAPTORS_H_

#include <llvm/IR/Function.h>

namespace ptsto {

template <typename Iterable>
class ReverseAdaptor {
public:
    using IterableIterator = typename Iterable::iterator;

    class iterator {
    public:
        iterator(IterableIterator beg)
            : beg_{ beg }, current_{ 0 }
        { }

        bool operator==(const IterableIterator &rhs) {
            return rhs.beg_ == beg_;
        }

        bool operator!=(const IterableIterator &rhs) {
            return !(*this == rhs);
        }

        iterator operator++() {
            return iterator(beg_ - ++current_);
        }

        iterator operator++(int) {
            iterator t = *this;
            ++(*this);
            return t;
        }

    private:
        IterableIterator beg_;
        unsigned current_;
    };

    ReverseAdaptor(Iterable &iterable)
    : iterable_{ iterable }
    { }

    iterator begin() {
        return iterable_.end() - 1;
    }

    iterator end() {
        return iterable_.begin() - 1;
    }

private:
    Iterable &iterable_;
};

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
