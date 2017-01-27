#include "Constraint.h"

#include <llvm/Support/raw_ostream.h>
namespace ptsto {

const char *typeToString[] = {
    "ADDRESS_OF",
    "LOAD",
    "STORE",
    "COPY"
};

void Constraint::dump() {
    llvm::errs() << typeToString[static_cast<int>(type_)]
        << "(" << source_ << ", " << dest_ << ")\n";
}

}
