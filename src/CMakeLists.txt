add_library(vk-lib STATIC core.cpp synchronization.cpp resources.cpp)

include_directories(../include)

find_package(Vulkan REQUIRED)

target_link_libraries(vk-lib PUBLIC Vulkan::Vulkan)

target_include_directories(vk-lib PUBLIC ../include)
