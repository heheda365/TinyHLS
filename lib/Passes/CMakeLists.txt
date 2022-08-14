

add_library(
  tinyHLSPasses
  CountIR.cpp 
  FindFunction.cpp
  DependenceList.cpp
  LoopInformationCollect.cpp
  SimpleTimingEvaluation.cpp
  SimpleTimingEvaluationBlockLatency.cpp
  SimpleTimingEvaluationFunctionLatency.cpp
  SimpleTimingEvaluationLoopLatency.cpp
)