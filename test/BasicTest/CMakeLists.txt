add_executable(TinyHLSTest
  Driver.cpp
)

target_link_libraries(TinyHLSTest
  PRIVATE
  ${llvm_libs}
  tinyHLSBasic
  tinyHLSUtils
)
