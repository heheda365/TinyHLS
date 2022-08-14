add_executable(SimpleTimingEvaluationTest
  SimpleTimingEvaluationTest.cpp 
)

target_link_libraries(SimpleTimingEvaluationTest
  PRIVATE
  ${llvm_libs}
  tinyHLSBasic
  tinyHLSUtils
  tinyHLSPasses
)
