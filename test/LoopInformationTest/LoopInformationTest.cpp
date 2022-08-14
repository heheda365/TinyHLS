#include "tinyhls/Passes/DependenceList.h"
#include "tinyhls/Passes/FindFunction.h"
#include "tinyhls/Passes/LoopInformationCollect.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Transforms/Utils/LoopSimplify.h"
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <memory>

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
      "clang -fno-discard-value-names -O1 -emit-llvm -S " + std::string(argv[1]) + " -o top.bc 2>&1";
  print_cmd(cmd_str.c_str());
  system(cmd_str.c_str());


  std::unique_ptr<Module> Mod(parseIRFile("top.bc", Err, Context));
  if (!Mod) {
    Err.print(argv[0], errs());
    return 1;
  }

  // Create a pass manager and fill it with the passes we want to run.
  legacy::PassManager PM;

  auto dominatortreewrapperpass = new DominatorTreeWrapperPass();
  PM.add(dominatortreewrapperpass);
  print_info("Enable DominatorTreeWrapperPass Pass");

  print_info("Enable HI_FindFunctions Pass");
  print_info("Enable HI_DependenceList Pass");
  auto findfunction = new FindFunctions();
  PM.add(findfunction);

  print_info("Enable LoopSimplify Pass");
  print_info("Enable IndVarSimplify Pass");

  print_info("Enable HI_LoopInFormationCollect Pass");
  auto loopinformationcollect = new LoopInFormationCollect("Loops");
  loopinformationcollect->solveDependency(PM);
  PM.add(loopinformationcollect);

  print_status("Start LLVM processing");
  PM.run(*Mod);
  print_status("Accomplished LLVM processing");

  print_status("Writing LLVM IR to File");
  std::error_code EC;
  llvm::raw_fd_ostream OS("top_output.bc", EC);
  
  OS << *Mod << "\n";
  OS.flush();

  OS.close();
  return 0;
}
