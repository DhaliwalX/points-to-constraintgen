#include "Variable.h"

#include <llvm/Support/Format.h>
#include <llvm/Support/FormatProviders.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/Debug.h>

#define DEBUG_TYPE "var"

// namespace llvm {

// template <>
// struct format_provider<pass::PointerType> {
//     static void format(const pass::PointerType &type, raw_ostream &stream, StringRef style) {
//         stream << static_cast<int>(type);
//     }
// };

// } // llvm

static const char *type[] = {
    "",
    "*",
    "&",
    "struct",
    "don't know"
};

namespace pass {
using namespace llvm;

void Variable::dump() {
    DEBUG(for (auto &value : *info_) {
        value->dump();
    });

    if (has_offset_) {
        errs() << format("%s %s(off=%d)%s", type[static_cast<int>(type_)],
                                    name_.c_str(), offset_,
                                    pointer_arithematic_ ? "+p" : "");
    } else {
        errs() << format("%s %s%s", type[static_cast<int>(type_)],
                                    name_.c_str(), pointer_arithematic_ ? "+p" : "");
    }
}

}
