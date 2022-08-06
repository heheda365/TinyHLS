#include "tinyhls/Passes/FindFunction.h"
#include "tinyhls/Utils/HI_print.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"

namespace tinyhls {
using namespace llvm;

bool FindFunctions::runOnModule(Module &M) {

  for (auto functionI = M.begin(); functionI != M.end(); ++ functionI) {
    auto functionName = functionI->getName();
    mFunctionDemangleMap[functionName] = demangleFunctionName(functionName);
  }

  return false;
}

char FindFunctions::ID = 0;
// the ID for pass should be initialized but the value does not matter,
// since LLVM uses the address of this variable as label instead of its
// value.

void FindFunctions::getAnalysisUsage(AnalysisUsage &AU) const {
  AU.setPreservesAll();
}

} // namespace tinyhls