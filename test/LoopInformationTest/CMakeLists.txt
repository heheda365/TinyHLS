add_executable(LoopInformationTest
  LoopInformationTest.cpp 
)

target_link_libraries(LoopInformationTest
  PRIVATE
  ${llvm_libs}
  tinyHLSBasic
  tinyHLSUtils
  tinyHLSPasses
)
