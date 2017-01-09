#ifndef VARIABLE_H_
#define VARIABLE_H_
#include <llvm/ADT/StringRef.h>
#include <llvm/IR/Value.h>

#include <cstddef>
#include "PointerInfo.h"

namespace pass {

enum class PointerType : int {
    kNormal = 0,
    kStar,
    kAmpersand,
    kStruct,
    kDontKnow
};

class Variable {
public:
    using ptrdiff_t = std::ptrdiff_t;

    Variable() = default;

    Variable(std::string name, llvm::Value *val, PointerInfo *info, bool rhs)
        : id_{ 0 }, offset_{ 0 }, pointer_arithematic_{ 0 }, type_ { PointerType::kDontKnow },
          rhs_{ rhs }, name_{name}, val_{ val }, info_{ info }
    { }

    ~Variable() = default;

    void dump();

    void setOffset(ptrdiff_t offset) {
        offset_ = offset;
    }

    void setActualOffset(ptrdiff_t actual_offset) {
        actual_offset_ = actual_offset;
    }

    void setPointerArithematic(ptrdiff_t arith) {
        pointer_arithematic_ = arith;
    }

    void setType(PointerType type) {
        type_ = type;
    }

    void setName(std::string name) {
        name_ = name;
    }

    void setHasOffset(bool value) {
        has_offset_ = value;
    }

    PointerInfo *getInfo() {
        return info_;
    }

    auto getId() -> unsigned int { return id_; }
private:
    // proces the pointer information i.e. how this variable actually comes into existence
    void processPointerInfo(); 

    unsigned int id_;
    bool rhs_;
    ptrdiff_t offset_;
    ptrdiff_t actual_offset_;
    bool has_offset_;
    ptrdiff_t pointer_arithematic_;
    PointerType type_;
    llvm::Value *val_;
    std::string name_;
    PointerInfo *info_;
};

}

#endif
