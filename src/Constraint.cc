#include "Constraint.h"

#include <llvm/IR/Instructions.h>

#include <cassert>

using namespace llvm;

namespace pass {

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

static uint64_t getFullOffset(GetElementPtrInst *instr) {
    uint64_t offset = 0;
    for (auto &index : UserAdaptor<GetElementPtrInst>(*instr)) {
        if (auto constant = dyn_cast<Constant>(index.get())) {
            offset += constant->getUniqueInteger().getSExtValue();
        }
    }

    return offset;
}

#define POINTER_POISON -1

void Constraint::processNode(ptrdiff_t &offset,
                            ptrdiff_t &pointer_arithematic,
                            ptrdiff_t &actual_offset,
                            PointerType &type,
                            std::string &name,
                            bool &offset_calculated,
                            PointerInfo *info) {
    
    if (info == nullptr) {
        errs() << "Unable to generate information for: " << name << "\n";
        return;
    }
    auto beg = info->rbegin();
    auto end = info->rend();

    name = ((*beg)->getName());
    while (beg != end) {
        auto val = *beg;
        type = PointerType::kStruct;

        if (auto gepinstr = dyn_cast<GetElementPtrInst>(val)) {
            if (gepinstr->getNumIndices() == 1) {
                // Example:
                //   getelementptr inbounds %struct.ST, %struct.ST* %0, i64 1
                // or
                //  %0 = load i32**, i32*** %arr, align 8
                //  %1 = load i64, i64* %i, align 8
                //  %arrayidx = getelementptr inbounds i32*, i32** %0, i64 %1

                // TODO := do we have to store the index as well

                // In LLVM, even if we just fetch a single element from structure
                // there is always an extra 0 index
                // Reference:
                //  http://llvm.org/docs/GetElementPtr.html#why-is-the-extra-0-index-required
                // So in LLVM pointer_arithematic should be true. 
                if (POINTER_POISON == pointer_arithematic)
                    pointer_arithematic = 0;

            } else {

                if (POINTER_POISON == pointer_arithematic)
                    pointer_arithematic = 1;
                
                if (gepinstr->getSourceElementType()->isStructTy()) {
                    auto indexValue = UserAdaptor<GetElementPtrInst>(*gepinstr).begin() + 1;
                    Constant *constant;
                    if (!offset_calculated
                        && (constant = dyn_cast<Constant>(indexValue->get()))) {
                        offset = constant->getUniqueInteger().getSExtValue();
                        offset_calculated = true;
                    }
                }
            }

            if ((*beg)->getName().str().length() != 0)
                name = name + std::string("[")
                            + std::string((*beg)->getName().str()) + std::string("]");
            actual_offset += getFullOffset(gepinstr);
        }
        ++beg;
    }
}

void Constraint::processLHS() {
    auto info = lhs_->getInfo();
    ptrdiff_t offset = 0;
    bool offset_calculated = false;
    ptrdiff_t pointer_arithematic = POINTER_POISON;
    ptrdiff_t actual_offset = 0;
    std::string name = "";
    PointerType type;

    assert(info);
    assert(info->size());

    if (info->size() == 1 && isa<AllocaInst>(*(info->begin()))) {
        type = (PointerType::kNormal);
        name = ((*(info->begin()))->getName().str());
        pointer_arithematic = 0;
    } else if (info->size() == 2 && isa<LoadInst>(*(info->begin()))) {
        type = (PointerType::kStar);
        name = ((*(--info->end()))->getName().str());
        pointer_arithematic = 0;
    } else {
        processNode(offset, pointer_arithematic, actual_offset,
                    type, name, offset_calculated, info);
    }


    lhs_->setPointerArithematic(pointer_arithematic);
    lhs_->setOffset(offset);
    lhs_->setHasOffset(offset_calculated);
    lhs_->setActualOffset(actual_offset);
    lhs_->setName(name);
    lhs_->setType(type);
}

void Constraint::processRHS() {
    auto info = rhs_->getInfo();
    ptrdiff_t offset = 0;
    bool offset_calculated = false;
    ptrdiff_t pointer_arithematic = POINTER_POISON;

    ptrdiff_t actual_offset = 0;
    std::string name;
    PointerType type;

    assert(info);
    assert(info->size());

    if (info->size() == 1 && isa<AllocaInst>(*(info->begin()))) {
        type = (PointerType::kAmpersand);
        name = ((*(info->begin()))->getName().str());
        pointer_arithematic = 0;
    } else if (info->size() == 2 && isa<LoadInst>(*(info->begin()))) {
        type = (PointerType::kNormal);
        name = ((*(--info->end()))->getName().str());
        pointer_arithematic = 0;     
    } else {
        processNode(offset, pointer_arithematic, actual_offset,
                    type, name, offset_calculated, info);
    }


    rhs_->setPointerArithematic(pointer_arithematic);
    rhs_->setOffset(offset);
    rhs_->setActualOffset(actual_offset);
    rhs_->setHasOffset(offset_calculated);
    rhs_->setName(name);
    rhs_->setType(type);
}

void Constraint::processConstraintInformation() {
    processRHS();
    processLHS();
}

}
