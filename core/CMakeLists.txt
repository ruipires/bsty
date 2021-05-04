project(bsty_core CXX)

add_library(bsty_core
            BstyData.h BstyData.cpp)

target_include_directories(bsty_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(bsty_core PRIVATE
        CONAN_PKG::spdlog # currently using FMT bundled with SPDLOG
        )
