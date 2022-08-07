#ifndef TINYHLS_PASS_LOOPINFORMATIONCOLLECT
#define TINYHLS_PASS_LOOPINFORMATIONCOLLECT

#include "llvm/Analysis/LoopAccessAnalysis.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/Analysis/OptimizationRemarkEmitter.h"
#include "llvm/IR/Dominators.h"
#include "llvm/IR/LegacyPassManager.h"
#include "tinyhls/Utils/HI_print.h"

namespace tinyhls {

class LoopInFormationCollect : public llvm::LoopPass {
public:
  LoopInFormationCollect(const char *loopfile) : LoopPass(ID) {
    //  this->getAsPMDataManager();
    Loop_Counter = 0;
    Loop_out = new llvm::raw_fd_ostream(loopfile, ErrInfo);
    // Loop2Blocks.clear();
    // Block2Loops.clear();
    // Loop_id.clear();
  } // define a pass, which can be inherited from ModulePass, LoopPass,
    // FunctionPass and etc.
  ~LoopInFormationCollect() {
    for (auto ele : Loop2Blocks) {
      delete ele.second;
    }
    for (auto ele : Block2Loops) {
      delete ele.second;
    }
    Loop_out->flush();
    delete Loop_out;
  }
  virtual bool doInitialization(llvm::Loop *L,
                                llvm::LPPassManager &LPPM) override {
    std::string tmpInfo =
        std::string("Initilizing LoopInFormationCollect pass for Loop ") +
        L->getName().str();
    print_status(tmpInfo.c_str());

    return false;
  }
  void printMaps() {
    *Loop_out << "\n=============content in maps================\n";
    for (auto ele : Loop2Blocks) {
      if (ele.first->isInvalid()) {
        print_info("Pass finalization encounter invalid loop");
        llvm::errs() << "Loop-ID:" << Loop_id[ele.first]
               << " is invalid when being printed by pass.\n";
        continue;
      }
      *Loop_out << "Loop: " << ele.first->getName() << " <--- Blocks: ";
      for (auto vec_ele : *ele.second) {
        *Loop_out << vec_ele->getName() << " ";
      }
      *Loop_out << "\n";
    }
    for (auto ele : Block2Loops) {
      *Loop_out << "Block: " << ele.first->getName() << " ---> Loops: ";
      for (auto vec_ele : *ele.second) {
        if (vec_ele->isInvalid()) {
          print_info("Pass finalization encounter invalid loop");
          llvm::errs() << "Loop-ID:" << Loop_id[vec_ele]
                 << " is invalid when being printed by pass.\n";
          continue;
        }
        *Loop_out << vec_ele->getName() << " ";
      }
      *Loop_out << "\n";
    }
  }
  virtual bool doFinalization() override{
    // print out the content in maps
    // printMaps();
    return false;
  }
  virtual void getAnalysisUsage(llvm::AnalysisUsage &AU) const override;
  virtual bool runOnLoop(llvm::Loop *L, llvm::LPPassManager &) override;

  static char ID;

  int Loop_Counter;

  std::map<llvm::Loop *, int> Loop_id;
  std::map<llvm::Loop *, std::vector<llvm::BasicBlock *> *> Loop2Blocks;
  std::error_code ErrInfo;
  llvm::raw_ostream *Loop_out;
  std::map<llvm::BasicBlock *, std::vector<llvm::Loop *> *> Block2Loops;

  llvm::ScalarEvolution *SE;
  llvm::LoopInfo *LI;
  llvm::LoopAccessLegacyAnalysis *LAA;

  void solveDependency(llvm::legacy::PassManager &PM) {

    auto loopinfowrapperpass = new llvm::LoopInfoWrapperPass();
    PM.add(loopinfowrapperpass);

    auto scalarevolutionwrapperpass = new llvm::ScalarEvolutionWrapperPass();
    PM.add(scalarevolutionwrapperpass);

    auto loopaccesslegacyanalysis = new llvm::LoopAccessLegacyAnalysis();
    PM.add(loopaccesslegacyanalysis);

    auto dominatortreewrapperpass = new llvm::DominatorTreeWrapperPass();
    PM.add(dominatortreewrapperpass);

    auto optimizationremarkemitterwrapperpass =
        new llvm::OptimizationRemarkEmitterWrapperPass();
    PM.add(optimizationremarkemitterwrapperpass);
  }
};

} // end of namespace tinyhls

#endif
