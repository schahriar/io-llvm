cmake_minimum_required(VERSION 3.7)

file(GLOB SOURCES ./*.cpp)

add_library(
  ast STATIC
  ${SOURCES}
)
