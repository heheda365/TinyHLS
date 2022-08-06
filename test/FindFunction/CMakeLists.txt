add_executable(FindFunction
  TestFindFunctions.cpp 
)

target_link_libraries(FindFunction
  PRIVATE
  ${llvm_libs}
  tinyHLSBasic
  tinyHLSUtils
  tinyHLSPasses
)
