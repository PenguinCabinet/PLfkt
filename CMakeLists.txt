cmake_minimum_required(VERSION 3.19.3)
SET(CMAKE_FIND_LIBRARY_PREFIXES "")
SET(CMAKE_FIND_LIBRARY_SUFFIXES ".lib" ".dll")

find_package(BISON)
find_package(FLEX)
find_package(LLVM REQUIRED CONFIG)


project(PLfkt CXX)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

BISON_TARGET(Parser src/parser.yy ${CMAKE_CURRENT_BINARY_DIR}/parser.tab.cpp)
FLEX_TARGET(Lex src/lex.l ${CMAKE_CURRENT_BINARY_DIR}/lex.tab.cpp COMPILE_FLAGS  "--header=${CMAKE_CURRENT_BINARY_DIR}/lex.tab.hpp")
ADD_FLEX_BISON_DEPENDENCY(Lex Parser)

add_executable(PLfkt ${BISON_Parser_OUTPUTS} ${FLEX_Lex_OUTPUTS} src/AST.hpp  src/AST.cpp src/mean_AST.hpp  src/mean_AST.cpp   src/IR_dump.hpp   src/IR_dump.cpp src/main.cpp)

llvm_map_components_to_libnames(llvm_libs support core irreader)
target_link_libraries(PLfkt ${llvm_libs})

#target_link_libraries(PLfkt ${FLEX_LIBRARIES})
#target_link_libraries(PLfkt ${BISON_Parser_OUTPUTS})
