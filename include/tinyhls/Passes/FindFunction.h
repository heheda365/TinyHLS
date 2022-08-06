#ifndef TINYHLS_PASS_FINDFUNCTION
#define TINYHLS_PASS_FINDFUNCTION

#include "llvm/ADT/StringMap.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include <cctype>
#include <cstddef>
#include <map>
#include <set>
#include <string>
namespace tinyhls {

class FindFunctions : public llvm::ModulePass {

public:
  FindFunctions() : ModulePass(ID){ }
  // define a pass, which can be inherited from ModulePass, LoopPass,
  // FunctionPass and etc.
  ~FindFunctions() { };
  virtual void getAnalysisUsage(llvm::AnalysisUsage &AU) const override;
  virtual bool runOnModule(llvm::Module &M) override;

  llvm::StringMap<llvm::StringRef> getFunctionDemangleMap() {
    return mFunctionDemangleMap;
  }

private:
  llvm::StringRef demangleFunctionName(llvm::StringRef mangled_name) {
    llvm::StringRef demangled_name;
    // demangle the function
    if (mangled_name.find("_Z") == llvm::StringRef::npos || 
        mangled_name.size() <= 2
    ) {
      demangled_name = mangled_name;
    } else {
      size_t index = 2;
      while (std::isdigit(mangled_name[index]) && index < mangled_name.size()) {
        index ++;
      }
      size_t demangeledNameLen = std::stoi(mangled_name.substr(2, index - 2).str());
      demangled_name = mangled_name.substr(index, demangeledNameLen);
    }
    return demangled_name;
  }


private:
  static char ID;
  llvm::StringMap<llvm::StringRef> mFunctionDemangleMap;
};

} // end namespace tinyhls

#endif
