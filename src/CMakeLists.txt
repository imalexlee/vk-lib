add_library(vk-lib STATIC core.cpp
        ../include/vk_lib/core.h)

find_package(Vulkan REQUIRED)
target_link_libraries(vk-lib PUBLIC Vulkan::Vulkan)

target_include_directories(vk-lib PUBLIC ../include)
