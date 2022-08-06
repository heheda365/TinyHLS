#include "tinyhls/Passes/FindFunction.h"
#include "tinyhls/Utils/HI_print.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Pass.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <system_error>

using namespace llvm;
using namespace tinyhls;

int main(int argc, char **argv) {
  if (argc < 2) {
    errs() << "Usage: " << argv[0] << " <C/C++ file>\n";
    return 1;
  }

  // Compile the source code into IR and Parse the input LLVM IR file into a
  // module
  SMDiagnostic Err;
  LLVMContext Context;
  std::string cmd_str =
      "clang -O1 -emit-llvm -S " + std::string(argv[1]) + " -o top.ll";
  print_cmd(cmd_str.c_str());
  system(cmd_str.c_str());

  std::unique_ptr<Module> Mod(parseIRFile("top.ll", Err, Context));
  if (!Mod) {
    Err.print(argv[0], errs());
    return 1;
  }

  // Create a pass manager and fill it with the passes we want to run.
  legacy::PassManager PM;
  FindFunctions *findFunctions = new FindFunctions();
  PM.add(findFunctions); 
  PM.run(*Mod);

  auto functionMap = findFunctions->getFunctionDemangleMap();
  std::error_code  errCode;
  raw_fd_ostream ofstream("functionNameMap.txt", errCode);

  for (auto& key: functionMap.keys()) {
    ofstream << key << " : " << functionMap[key] << "\n"; 
  }

  ofstream.close();

  return 0;
}
