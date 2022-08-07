add_executable(TestDependence
  TestDependence.cpp
)

target_link_libraries(TestDependence
  PRIVATE
  ${llvm_libs}
  tinyHLSBasic
  tinyHLSUtils
  tinyHLSPasses
)
