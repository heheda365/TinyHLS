#ifndef TINYHLS_PASS_SimpleTimingEvaluation
#define TINYHLS_PASS_SimpleTimingEvaluation

#include "llvm/Analysis/LoopAccessAnalysis.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/Pass.h"
#include <set>
#include "tinyhls/Passes/LoopInformationCollect.h"
#include "tinyhls/Utils/HI_print.h"

// Pass for simple evluation of the latency of the top function, without
// considering HLS directives
class SimpleTimingEvaluation : public llvm::ModulePass {
public:
  // Pass for simple evluation of the latency of the top function, without
  // considering HLS directives
  SimpleTimingEvaluation(const char *evaluating_log_name,
                            const char *top_function)
      : ModulePass(ID) {
    BlockEvaluated.clear();
    LoopEvaluated.clear();
    FunctionEvaluated.clear();
    Loop_id.clear();
    Loop_Counter = 0;
    Evaluating_log =
        new llvm::raw_fd_ostream(evaluating_log_name, ErrInfo);
    top_function_name = std::string(top_function);
  } // define a pass, which can be inherited from ModulePass, LoopPass,
    // FunctionPass and etc.
  ~SimpleTimingEvaluation() {
    for (auto ele : Loop2Blocks) {
      delete ele.second;
    }
    for (auto ele : Block2Loops) {
      delete ele.second;
    }
    Evaluating_log->flush();
    delete Evaluating_log;
  }

  virtual bool doInitialization(llvm::Module &M) override {

    print_status("Initilizing SimpleTimingEvaluation pass.");
    Loop_id.clear();
    LoopLatency.clear();
    BlockLatency.clear();
    FunctionLatency.clear();
    BlockEvaluated.clear();
    Func_BlockEvaluated.clear();
    LoopEvaluated.clear();
    FunctionEvaluated.clear();
    InstructionEvaluated.clear();
    BlockVisited.clear();
    Func_BlockVisited.clear();

    Function2OuterLoops.clear();
    Block2EvaluatedLoop.clear();
    BlockCriticalPath_inLoop.clear();
    tmp_BlockCriticalPath_inFunc.clear();
    tmp_LoopCriticalPath_inFunc.clear();
    tmp_BlockCriticalPath_inLoop.clear();
    tmp_SubLoop_CriticalPath.clear();
    InstructionCriticalPath_inBlock.clear();

    return false;
  }

  void getAnalysisUsage(llvm::AnalysisUsage &AU) const override;
  virtual bool runOnModule(llvm::Module &M) override;

  // check whether the block is in some loops
  bool isInLoop(llvm::BasicBlock *BB);

  // evaluatate the latency of a outer loop, which could be a nested one
  double getOuterLoopLatency(llvm::Loop *outerL);

  // get the most outer loop which contains the block, treat the loop as a node
  // for the evaluation of latency
  llvm::Loop *getOuterLoopOfBlock(llvm::BasicBlock *B);

  // find the inner unevaluated loop for processing
  llvm::Loop *getInnerUnevaluatedLoop(llvm::Loop *outerL);

  // evaluate a loop in which all the children loops have been evauluated
  double getLoopLatency_InnerChecked(llvm::Loop *L);

  static char ID;

  int Loop_Counter;

  double top_function_latency = 0.0;

  std::map<llvm::Loop *, int> Loop_id;

  // the latency of each loop
  std::map<llvm::Loop *, double> LoopLatency;

  // the latency of each block
  std::map<llvm::BasicBlock *, double> BlockLatency;

  // the latency of each function
  std::map<llvm::Function *, double> FunctionLatency;

  // record whether the component is evaluated
  std::set<llvm::BasicBlock *> BlockEvaluated;
  std::set<llvm::BasicBlock *> Func_BlockEvaluated;
  std::set<llvm::Loop *> LoopEvaluated;
  std::set<llvm::Function *> FunctionEvaluated;
  std::set<llvm::Instruction *> InstructionEvaluated;
  std::set<llvm::BasicBlock *> BlockVisited;
  std::set<llvm::BasicBlock *> Func_BlockVisited;

  // record the information of the processing
  llvm::raw_ostream *Evaluating_log;
  std::error_code ErrInfo;

  // the pass requires a specified top_function name
  std::string top_function_name;

  // record the relations between loops and blocks
  std::map<llvm::Loop *, std::vector<llvm::BasicBlock *> *> Loop2Blocks;
  std::map<llvm::BasicBlock *, std::vector<llvm::Loop *> *> Block2Loops;

  // record the list of outer loops for functions
  std::map<llvm::Function *, std::vector<llvm::Loop *>> Function2OuterLoops;

  // record which evaluated loop the block is belong to, so the pass can
  // directly trace to the loop for the latency
  std::map<llvm::BasicBlock *, llvm::Loop *> Block2EvaluatedLoop;

  // record the critical path from the loop header to the end of the specific
  // block
  std::map<llvm::Loop *, std::map<llvm::BasicBlock *, double>>
      BlockCriticalPath_inLoop;

  // record the critical path to the end of block in the function
  std::map<llvm::BasicBlock *, double> tmp_BlockCriticalPath_inFunc;

  // record the critical path to the end of loops in the function
  std::map<llvm::Loop *, double> tmp_LoopCriticalPath_inFunc;

  // record the critical path to the end of sub-loops in the loop
  std::map<llvm::BasicBlock *, double> tmp_BlockCriticalPath_inLoop;

  // record the critical path from the outter loop header to the end of the
  // specific sub-loop
  std::map<llvm::Loop *, double> tmp_SubLoop_CriticalPath;

  // record the critical path from the block entry to the end of the specific
  // instruction
  std::map<llvm::BasicBlock *, std::map<llvm::Instruction *, double>>
      InstructionCriticalPath_inBlock;

  // get the function latency
  double getFunctionLatency(llvm::Function *F);

  // get the function critical path by traversing the blocks based on DFS
  void getFunctionLatency_traverseFromEntryToExiting(
      double tmp_critical_path, llvm::Function *F, llvm::BasicBlock *curBlock);

  // get the loop latency by traversing from the header to the exiting blocks
  void LoopLatencyEvaluation_traversFromHeaderToExitingBlocks(
      double tmp_critical_path, llvm::Loop *L, llvm::BasicBlock *curBlock);

  // mark the block in loop with latency by traversing from the header to the
  // exiting blocks
  void MarkBlock_traversFromHeaderToExitingBlocks(double total_latency,
                                                  llvm::Loop *L,
                                                  llvm::BasicBlock *curBlock);

  // evaluate the block latency by traversing the instructions
  double BlockLatencyEvaluation(llvm::BasicBlock *B);

  // check whether the instruction is in the block
  bool BlockContain(llvm::BasicBlock *B, llvm::Instruction *I);

  // get the latency of a specific instruction
  double getInstructionLatency(llvm::Instruction *I);

  // check whether all the sub-function are evaluated
  bool CheckDependencyFesilility(llvm::Function &F);

  // get the relationship between loops and blocks
  void getLoopBlockMap(llvm::Function *F);

  // some LLVM analysises could be involved
  llvm::ScalarEvolution *SE;
  llvm::LoopInfo *LI;
  llvm::LoopAccessLegacyAnalysis *LAA;

  bool topFunctionFound = 0;
  //  std::map<>

  /// Timer

  struct timeval tv_begin;
  struct timeval tv_end;
};

#endif
