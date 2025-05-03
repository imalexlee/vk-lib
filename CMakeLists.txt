cmake_minimum_required(VERSION 3.25)

project(vk_lib)
set(CMAKE_CXX_STANDARD 20)

# Used for my own subdirectories to pull dependencies
include(FetchContent)

find_package(Vulkan REQUIRED)

option(VK_LIB_BUILD_EXAMPLES "Build example application" OFF)
option(VK_LIB_BUILD_TESTS "Build tests" OFF)

add_subdirectory(src)

if (VK_LIB_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif ()
if (VK_LIB_BUILD_TESTS)
    add_subdirectory(tests)
endif ()

