cmake_minimum_required(VERSION 2.8.12)
set(CMAKE_CXX_STANDARD 17)
project(bsty CXX)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

# all subprojects have access to the root of the source tre
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(core)

add_library(bsty_util
        util/CodePageToUtf8Converter.cpp util/CodePageToUtf8Converter.h
        util/DateParser.cpp util/DateParser.h)
target_include_directories(bsty_util PUBLIC .)

target_link_libraries(bsty_util PUBLIC
        CONAN_PKG::icu
        CONAN_PKG::spdlog
        CONAN_PKG::date
        bsty_core
        )

add_library(bsty_cgd OBJECT
            cgd/ContaOrdem.cpp cgd/ContaOrdem.h)

target_link_libraries(bsty_cgd PUBLIC
        CONAN_PKG::fast-cpp-csv-parser
        CONAN_PKG::spdlog # should not be needed
        bsty_util
        bsty_core # should not be needed as it is a transient dependency of bsty_util
        )

add_executable(bsty main.cpp
        Configuration.cpp Configuration.h
        )

target_link_libraries(bsty PRIVATE
        CONAN_PKG::cxxopts
        CONAN_PKG::nlohmann_json
        bsty_util
        bsty_cgd
        )

add_executable(ut
        ut/main.cpp
        ut/CodePageToUtf8Converter.ut.cpp
        ut/DateParser.ut.cpp
        ut/ContaOrdem.ut.cpp
        )

target_link_libraries(ut PRIVATE
        bsty_util
        bsty_cgd
        CONAN_PKG::catch2
        )

configure_file(ut/windows-1252.sample.txt ${CMAKE_BINARY_DIR} COPYONLY)
configure_file(${CONAN_RES_DIRS_ICU}/icudt68l.dat ${CMAKE_BINARY_DIR} COPYONLY)
