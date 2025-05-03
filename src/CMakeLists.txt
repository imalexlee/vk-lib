add_library(vk-lib STATIC core.cpp synchronization.cpp resources.cpp shaders.cpp presentation.cpp commands.cpp shader_data.cpp pipelines.cpp rendering.cpp)

include_directories(../include)

target_include_directories(vk-lib PUBLIC ../include)

target_link_libraries(vk-lib Vulkan::Vulkan)