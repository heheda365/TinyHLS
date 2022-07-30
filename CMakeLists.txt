cmake_minimum_required(VERSION 3.4.3)
project(Tinylang)

# setting LLVM
find_package(LLVM REQUIRED CONFIG)
list(APPEND CMAKE_MODULE_PATH ${LLVM_DIR})
include(ChooseMSVCCRT)
include(DetermineGCCCompatible)
include(AddLLVM)
include(HandleLLVMOptions)

include_directories("${LLVM_BINARY_DIR}/include" "${LLVM_INCLUDE_DIR}")
link_directories("${LLVM_LIBRARY_DIR}")
llvm_map_components_to_libnames(llvm_libs Core Support)
if(LLVM_COMPILER_IS_GCC_COMPATIBLE)
  if(NOT LLVM_ENABLE_RTTI)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
  endif()
  if(NOT LLVM_ENABLE_EH)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions")
  endif()
endif()
# end of setting LLVM 


set(TINYHLS_BUILT_STANDALONE 1)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")
include(AddTinyHLS)

set(TINYHLS_VERSION_STRING "0.1")
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/include/tinyhls/Basic/Version.inc.in
  ${CMAKE_CURRENT_BINARY_DIR}/include/tinyhls/Basic/Version.inc)


include_directories(BEFORE
  ${CMAKE_CURRENT_BINARY_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  )

add_subdirectory(lib)
add_subdirectory(test)