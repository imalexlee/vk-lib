add_library(vk-lib STATIC core.cpp synchronization.cpp resources.cpp shaders.cpp presentation.cpp commands.cpp shader_data.cpp vma_usage.cpp)

include_directories(../include)

target_include_directories(vk-lib PUBLIC ../include)

# current link is for Vulkan Memory Allocator 3.2.1
FetchContent_Declare(
        vma
        URL https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator/archive/refs/tags/v3.2.1.zip
)
FetchContent_MakeAvailable(vma)

# current link is for volk 1.4.304
FetchContent_Declare(
        volk
        URL https://github.com/zeux/volk/archive/refs/tags/1.4.304.zip
)
FetchContent_MakeAvailable(volk)

target_link_libraries(vk-lib PUBLIC GPUOpen::VulkanMemoryAllocator volk)
