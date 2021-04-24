cmake_minimum_required(VERSION 2.8.12)
set(CMAKE_CXX_STANDARD 17)
project(bsty CXX)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

add_subdirectory(core)

add_library(bsty_util OBJECT
        util/CodePageToUtf8Converter.cpp util/CodePageToUtf8Converter.h
        util/DateParser.cpp util/DateParser.h)
target_include_directories(bsty_util PUBLIC .)

target_link_libraries(bsty_util PUBLIC
        CONAN_PKG::icu
        CONAN_PKG::spdlog
        CONAN_PKG::date
        bsty_core
        )

add_executable(bsty main.cpp
        Configuration.cpp Configuration.h
        cgd/ContaOrdem.cpp cgd/ContaOrdem.h)

target_link_libraries(bsty PRIVATE
        CONAN_PKG::cxxopts
        CONAN_PKG::nlohmann_json
        CONAN_PKG::fast-cpp-csv-parser
        CONAN_PKG::spdlog
        bsty_util
        )

add_executable(ut
        ut/main.cpp
        ut/CodePageToUtf8Converter.ut.cpp ut/DateParser.ut.cpp)

configure_file(ut/windows-1252.sample.txt ${CMAKE_BINARY_DIR} COPYONLY)
configure_file(${CONAN_RES_DIRS_ICU}/icudt68l.dat ${CMAKE_BINARY_DIR} COPYONLY)

target_link_libraries(ut PRIVATE
        bsty_util
        CONAN_PKG::catch2
        )