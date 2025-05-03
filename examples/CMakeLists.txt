# current link is for GLFW 3.4
FetchContent_Declare(
        GLFW
        URL https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.zip
)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# current link is for volk 1.4.304
FetchContent_Declare(
        volk
        URL https://github.com/zeux/volk/archive/refs/tags/1.4.304.zip
)

FetchContent_MakeAvailable(volk)
FetchContent_MakeAvailable(GLFW)

link_libraries(vk-lib glfw volk)

add_executable(triangle triangle.cpp)
