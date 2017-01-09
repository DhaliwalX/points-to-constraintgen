#ifndef POINTER_INFO_H_
#define POINTER_INFO_H_

#include <llvm/IR/Value.h>

#include <vector>

namespace pass {
 
class PointerInfo {
public:
	using iterator = std::vector<llvm::Value *>::iterator;
	using const_iterator = std::vector<llvm::Value *>::const_iterator;
    using reverse_iterator = std::vector<llvm::Value *>::reverse_iterator;

	PointerInfo(std::vector<llvm::Value *> vec)
		: vec_{ vec }
	{ }

	iterator begin() {
		return vec_.begin();
	}

	iterator end() {
		return vec_.end();
	}

    reverse_iterator rbegin() {
        return vec_.rbegin();
    }

    reverse_iterator rend() {
        return vec_.rend();
    }

    std::vector<llvm::Value *>::size_type size() const {
        return vec_.size();
    }
private:
	std::vector<llvm::Value *> vec_;
};

}

#endif
