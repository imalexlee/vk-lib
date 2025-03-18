include(FetchContent)

add_library(vk-lib STATIC core.cpp synchronization.cpp resources.cpp vma_usage.cpp)

include_directories(../include)

target_include_directories(vk-lib PUBLIC ../include)

# current link is for Vulkan Memory Allocator 3.2.1
FetchContent_Declare(
        vma
        URL https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator/archive/refs/tags/v3.2.1.zip
)

FetchContent_MakeAvailable(vma)

target_link_libraries(vk-lib PRIVATE Vulkan::Vulkan GPUOpen::VulkanMemoryAllocator)
