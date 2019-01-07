cmake_minimum_required(VERSION 3.7)
project(IOLang)

set(CMAKE_CXX_COMPILER clang++)
add_definitions("-std=c++17")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# LLVM Package
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
# LLVM Includes
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_subdirectory(src/writers)
add_subdirectory(src/utils)
add_subdirectory(src/AST)
add_subdirectory(src/CodeGen)
add_executable(
  vm src/vm.cpp
)

# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(llvm_libs support core irreader bitwriter)

# Define dependencies
target_include_directories(codegen PUBLIC src)
add_dependencies(codegen ast)

# Link internal libraries
target_link_libraries(vm writers)
target_link_libraries(vm ast)
target_link_libraries(vm codegen)
target_link_libraries(vm utils)
# Link against LLVM libraries
target_link_libraries(vm ${llvm_libs})
# Link Conan libraries
target_link_libraries(vm ${CONAN_LIBS})

# Formatter
find_program(ASTYLE_EXECUTABLE astyle DOC "Automatic formatting tool")
add_custom_target("format" COMMAND
  "${ASTYLE_EXECUTABLE}"
  --indent-namespaces --indent=spaces=2 --style=google
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/language/*.hpp
  VERBATIM
)
