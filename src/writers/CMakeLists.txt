cmake_minimum_required(VERSION 3.9)

file(GLOB SOURCES ./*.cpp)

add_library(
  writers STATIC
  ${SOURCES}
)
