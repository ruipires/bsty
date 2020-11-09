cmake_minimum_required(VERSION 2.8.12)
set(CMAKE_CXX_STANDARD 17)
project(bsty CXX)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(bsty main.cpp main.cpp Configuration.cpp Configuration.h cgd/ContaOrdem.cpp cgd/ContaOrdem.h)
target_link_libraries(bsty ${CONAN_LIBS})

