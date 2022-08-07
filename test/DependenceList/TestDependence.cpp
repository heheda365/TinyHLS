#include "tinyhls/Passes/DependenceList.h"
#include "tinyhls/Utils/HI_print.h"
#include "llvm/IR/Dominators.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SourceMgr.h"

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
      "clang -fno-discard-value-names -O1 -emit-llvm -S " + std::string(argv[1]) + " -o top.ll";
  print_cmd(cmd_str.c_str());
  system(cmd_str.c_str());

  std::unique_ptr<Module> Mod(parseIRFile("top.ll", Err, Context));
  if (!Mod) {
    Err.print(argv[0], errs());
    return 1;
  }

  // Create a pass manager and fill it with the passes we want to run.
  legacy::PassManager PM;
  // auto dominatortreewrapperpass = new DominatorTreeWrapperPass();
  // PM.add(dominatortreewrapperpass); 

  DependenceList *dependencePass = new DependenceList();
  PM.add(dependencePass); 
  PM.run(*Mod);

 
  // std::error_code  errCode;
  // raw_fd_ostream ofstream("functionNameMap.txt", errCode);

  // for (auto& key: functionMap.keys()) {
  //   ofstream << key << " : " << functionMap[key] << "\n"; 
  // }

  // ofstream.close();

  return 0;
}