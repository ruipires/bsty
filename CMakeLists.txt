cmake_minimum_required(VERSION 2.8.12)
set(CMAKE_CXX_STANDARD 17)
project(bsty CXX)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

add_library(bsty_core
        Configuration.cpp Configuration.h
        cgd/ContaOrdem.cpp cgd/ContaOrdem.h)

target_link_libraries(bsty_core PUBLIC
        CONAN_PKG::cxxopts
        CONAN_PKG::nlohmann_json
        CONAN_PKG::fast-cpp-csv-parser
        CONAN_PKG::spdlog
        )

add_executable(bsty
        main.cpp)

target_link_libraries(bsty PRIVATE bsty_core)

add_executable(ut
        ut/main.cpp ut/cgd.ContaOrdem.ut.cpp)

target_include_directories(ut PRIVATE .)

target_link_libraries(ut PRIVATE
        bsty_core
        CONAN_PKG::catch2
        )

