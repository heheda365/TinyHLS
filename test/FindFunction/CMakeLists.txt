add_executable(FindFunction
  LLVM_exp0_find_functions.cc
)

target_link_libraries(FindFunction
  PRIVATE
  ${llvm_libs}
  tinyHLSBasic
  tinyHLSUtils
  tinyHLSPasses
)
