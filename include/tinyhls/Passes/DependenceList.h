#ifndef TINYHLS_PASS_DEPENDENCELIST
#define TINYHLS_PASS_DEPENDENCELIST

#include "tinyhls/Utils/HI_print.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/Value.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

namespace tinyhls {

class DependenceList : public llvm::FunctionPass {
public:
  DependenceList()
      : FunctionPass(ID),
        mInstCounter(0), 
        mFuncCounter(0),
        mBBlockCounter(0) { } 

  ~DependenceList() {
    for (auto& it : mBBlockSuccessors) {
      delete it.second;
    }
    for (auto& it: mBBlockPredecessors) {
      delete it.second;
    }
  }

  virtual bool doInitialization(llvm::Module &M) override {
    llvm::outs() << "doInitialization\n";
    return false;
  }

  virtual bool doFinalization(llvm::Module &M) override {
    llvm::outs() << "doFinalization \n";
    return false;
  }
  virtual void getAnalysisUsage(llvm::AnalysisUsage &AU) const override;
  virtual bool runOnFunction(llvm::Function &M) override;

  // find all the successors of the block and map them
  void checkSuccessorsOfBlock(llvm::BasicBlock *B);

  // find all the predecessors of the block and map them
  void checkPredecessorsOfBlock(llvm::BasicBlock *B);

  // find the dependence between instructions and map them
  void checkInstructionDependence(llvm::Instruction *I);

  // ID to T, T to ID
  template<typename T>
  void bindId4T(std::vector<T *> &id2T, std::map<T *, unsigned> &t2Id, T &t, unsigned &id);

private:
  static char ID;

  unsigned mInstCounter;
  unsigned mFuncCounter;
  unsigned mBBlockCounter;
  unsigned mLoopCounter;

  // mapping T to id, id to T
  std::map<llvm::Function *, unsigned> mFunc2Id;
  std::vector<llvm::Function *> mId2Func;

  std::map<llvm::BasicBlock *, unsigned> mBBlock2Id;
  std::vector<llvm::BasicBlock *> mId2BBlock;

  std::map<llvm::Instruction *, unsigned> mInst2Id;
  std::vector<llvm::Instruction *> mId2Inst;

  std::map<llvm::BasicBlock *, std::vector<llvm::BasicBlock *> *>
      mBBlockSuccessors;
  std::map<llvm::BasicBlock *, std::vector<llvm::BasicBlock *> *>
      mBBlockPredecessors;
  
  std::map<llvm::Instruction *, std::vector<llvm::Instruction *> *>
      mInstUsers;

};

} // end of namespace tinyhls
#endif
