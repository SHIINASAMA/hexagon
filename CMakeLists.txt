cmake_minimum_required(VERSION 3.13)

set(VCPKG_OVERLAY_PORTS "${CMAKE_CURRENT_LIST_DIR}/vcpkg-ports")

project(Hexagon)
set(CMAKE_CXX_STANDARD 20)

if (MSVC)
    add_compile_options(/utf-8)
endif ()

if (MSVC AND VCPKG_TARGET_TRIPLET STREQUAL "x64-windows-static")
    set(CMAKE_CXX_FLAGS_DEBUG /MTd)
    set(CMAKE_CXX_FLAGS_MINSIZEREL /MT)
    set(CMAKE_CXX_FLAGS_RELEASE /MT)
    set(CMAKE_CXX_FLAGS_RELWITHDEBUGINFO /MT)
endif ()

add_definitions(-DPROJECT_SOURCE_PATH=\"${PROJECT_SOURCE_DIR}\")

file(GLOB_RECURSE sources "src/cpp/*.cpp" "src/cpp/*.hpp")
add_executable(Hexagon ${sources})

find_package(sese CONFIG REQUIRED)
target_link_libraries(Hexagon PRIVATE Sese::Core)
