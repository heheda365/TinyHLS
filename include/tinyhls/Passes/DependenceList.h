#ifndef TINYHLS_PASS_DEPENDENCELIST
#define TINYHLS_PASS_DEPENDENCELIST
// related headers should be included.
#include "tinyhls/Utils/HI_print.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constant.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include <map>
#include <set>
#include <sstream>
#include <string>

namespace tinyhls {

class DependenceList : public llvm::FunctionPass {
public:
  DependenceList(const char *Instruction_out_file,
                    const char *dependence_out_file)
      : FunctionPass(ID) {
    Instruction_Counter = 0;
    Function_Counter = 0;
    BasicBlock_Counter = 0;
    Loop_Counter = 0;
    callCounter = 0;
    Instruction_out = new llvm::raw_fd_ostream(Instruction_out_file, ErrInfo);
    Dependence_out = new llvm::raw_fd_ostream(dependence_out_file, ErrInfo);
    tmp_stream = new llvm::raw_string_ostream(tmp_stream_str);
  } // define a pass, which can be inherited from ModulePass, LoopPass,
    // FunctionPass and etc.

  ~DependenceList() {
    for (auto it : Instruction2User_id) {
      delete it.second;
    }
    for (auto it : Blcok2InstructionList_id) {
      delete it.second;
    }
    for (auto it : Block_Successors) {
      delete it.second;
    }
    for (auto it : Block_Predecessors) {
      delete it.second;
    }
    Instruction_out->flush();
    delete Instruction_out;
    Dependence_out->flush();
    delete Dependence_out;
    tmp_stream->flush();
    delete tmp_stream;
  }

  virtual bool doInitialization(llvm::Module &M) override {
    print_status("Initilizing DependenceList pass.");
    for (auto it : Instruction2Pre_id) {
      delete it.second;
    }
    for (auto it : Instruction2User_id) {
      delete it.second;
    }
    for (auto it : Blcok2InstructionList_id) {
      delete it.second;
    }
    for (auto it : Block_Successors) {
      delete it.second;
    }
    for (auto it : Block_Predecessors) {
      delete it.second;
    }
    Function_id.clear();
    Instruction_id.clear();
    InstructionsNameSet.clear();
    BasicBlock_id.clear();
    Loop_id.clear();
    Block_Successors.clear();
    Block_Predecessors.clear();

    Instruction2Blcok_id.clear();
    Instruction2User_id.clear();
    Instruction2Pre_id.clear();
    Blcok2InstructionList_id.clear();

    return false;
  }

  virtual bool doFinalization(llvm::Module &M) override {
    InstructionsNameSet.clear();
    *Instruction_out
        << "\n\n\n\n\n=================== Instruction Set and Example "
           "======================\n\n";
    for (auto &F : M)
      for (auto &B : F)
        for (auto &I : B) {
          tmp_stream_str.clear();
          tmp_stream->flush();
          *tmp_stream << I.getOpcodeName() << " ";
          for (int i = 0; i < I.getNumOperands(); i++) {
            llvm::Value *tmp_op = I.getOperand(i);
            if (llvm::isa<llvm::Constant>(tmp_op)) {
              *tmp_stream << "op" << i << "_const:";
            } else {
              *tmp_stream << "op" << i << "_type:";
            }
            tmp_op->getType()->print(*tmp_stream);
            *tmp_stream << " ";
          }
          std::string checker = tmp_stream->str();
          if (InstructionsNameSet.find(checker) == InstructionsNameSet.end()) {
            *Instruction_out << "I-ID:" << Instruction_id[&I] << "-->"
                             << checker << "\n";
            InstructionsNameSet.insert(checker);
          }
        }
    return false;
  }
  void getAnalysisUsage(llvm::AnalysisUsage &AU) const override;
  virtual bool runOnFunction(llvm::Function &M) override;
  static char ID;

  // find all the successors of the block and map them
  void checkSuccessorsOfBlock(llvm::BasicBlock *B);

  // find all the predecessors of the block and map them
  void checkPredecessorsOfBlock(llvm::BasicBlock *B);

  // find the dependence between instructions and map them
  void checkInstructionDependence(llvm::Instruction *I);

  int callCounter;
  int Instruction_Counter;
  int Function_Counter;
  int BasicBlock_Counter;
  int Loop_Counter;

  llvm::Function *TargeFunction;

  std::map<llvm::Function *, int> Function_id;
  std::map<llvm::Instruction *, int> Instruction_id;
  std::set<std::string> InstructionsNameSet;
  std::map<llvm::BasicBlock *, int> BasicBlock_id;
  std::map<llvm::Loop *, int> Loop_id;
  std::map<llvm::BasicBlock *, std::vector<llvm::BasicBlock *> *>
      Block_Successors;
  std::map<llvm::BasicBlock *, std::vector<llvm::BasicBlock *> *>
      Block_Predecessors;

  std::map<int, int> Instruction2Blcok_id;
  std::map<int, std::vector<int> *> Instruction2User_id;
  std::map<int, std::vector<int> *> Instruction2Pre_id;
  std::map<int, std::vector<int> *> Blcok2InstructionList_id;

  std::error_code ErrInfo;
  llvm::raw_ostream *Instruction_out;
  llvm::raw_ostream *Dependence_out;

  llvm::raw_string_ostream *tmp_stream;
  std::string tmp_stream_str;

  /// Timer

  struct timeval tv_begin;
  struct timeval tv_end;
};

}  // end of namespace tinyhls
#endif
