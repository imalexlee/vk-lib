cmake_minimum_required(VERSION 3.29)

project(VK_LIB)
set(CMAKE_CXX_STANDARD 20)
set(VK_LIB_BUILD_EXAMPLES TRUE)

add_subdirectory(src)

if (VK_LIB_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
