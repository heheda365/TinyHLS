#include "tinyhls/Passes/DependenceList.h"
#include "llvm/IR/Dominators.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Value.h"
#include "llvm/Support/raw_ostream.h"
#include <cstddef>
#include <vector>

namespace tinyhls {

using namespace llvm;

template <typename T>
void DependenceList::bindId4T(std::vector<T *> &id2T,
                              std::map<T *, unsigned> &t2Id, T &t,
                              unsigned &id) {
  if (t2Id.count(&t)) {
    id2T.push_back(&t);
    t2Id[&t] = id;
    id++;
  }
}

bool DependenceList::runOnFunction(Function &F) {
  bindId4T<Function>(mId2Func, mFunc2Id, F, mFuncCounter);
  for (BasicBlock &B : F) {
    bindId4T<BasicBlock>(mId2BBlock, mBBlock2Id, B, mBBlockCounter);
    checkPredecessorsOfBlock(&B);
    checkSuccessorsOfBlock(&B);
    outs() << B << "\n";
    for (Instruction &I : B) {
      bindId4T<Instruction>(mId2Inst, mInst2Id, I, mInstCounter);
      checkInstructionDependence(&I);
    }
  }
  return false;
}

char DependenceList::ID = 0;

void DependenceList::getAnalysisUsage(AnalysisUsage &AU) const {
  AU.setPreservesAll();
}

void DependenceList::checkSuccessorsOfBlock(BasicBlock *B) {

  auto tmpVec = new std::vector<BasicBlock *>();
  for (auto succ : successors(B)) {
    tmpVec->push_back(succ);
  }
  mBBlockSuccessors[B] = tmpVec;
}

void DependenceList::checkPredecessorsOfBlock(BasicBlock *B) {

  auto tmpVec = new std::vector<BasicBlock *>();
  for (auto pred : predecessors(B)) {
    tmpVec->push_back(pred);
  }
  mBBlockPredecessors[B] = tmpVec;
}

void DependenceList::checkInstructionDependence(Instruction *I) {

  outs() << *I << "--> : \n";
  // who use the result output by I
  auto users = new std::vector<Instruction *>();
  for (auto U : I->users()) {
    if (auto succInst = dyn_cast<Instruction>(U)) {
      users->push_back(succInst);
      outs() << *succInst << "\n";
    }
  }

  // test operands 
  for (size_t i = 0; i < I->getNumOperands(); ++ i) {
    outs() << "        op#" << i << ": " << *I->getOperand(i);
    if (Argument *arg = dyn_cast<Argument>(I->getOperand(i))) {
      outs() << " is a function argument of function ("
                      << arg->getParent()->getName() << ").";
    } else {
      if (Instruction *InstTmp = dyn_cast<Instruction>(I->getOperand(i))) {
        outs() << " is an instruction.";
      } else {
        if (Constant *constVal = dyn_cast<Constant>(I->getOperand(i))) {
          outs() << " is a constant.";
        } else {
          outs() << " is a value with unknown type.";
        }
      }
    }
    outs() << " (type=";
    I->getOperand(i)->getType()->print(outs());
    outs() << " is ";
    if (I->getOperand(i)->getType()->isArrayTy()) {
      outs() << " array type)\n";
    } else {
      if (I->getOperand(i)->getType()->isPointerTy()) {
        outs() << " pointerTy type of type ";
        PointerType *tmp_PtrType =
            dyn_cast<PointerType>(I->getOperand(i)->getType());
        tmp_PtrType->getElementType()->print(outs());
        outs() << " )\n";
        // tmp_PtrType->gettype
      }
    }

    outs() << ".\n";
  }
}

} // end of namespace tinyhls