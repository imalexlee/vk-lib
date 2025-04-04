/*
 * Utilities regarding synchronization primitives and functions
 */

#pragma once
#include <vk_lib/common.h>

namespace vk_lib {
[[nodiscard]] VkSemaphoreCreateInfo semaphore_create_info(const void* pNext = nullptr);

[[nodiscard]] VkFenceCreateInfo fence_create_info(VkFenceCreateFlags flags = 0, const void* pNext = nullptr);

/*
 * CORE EXTENSIONS
 */

// VULKAN 1.3

[[nodiscard]] VkSemaphoreSubmitInfoKHR semaphore_submit_info(VkSemaphore semaphore, VkPipelineStageFlags2KHR stage_mask, uint64_t timeline_value = 0,
                                                             uint32_t device_index = 0);

// subresource_range MUST NOT be null
[[nodiscard]] VkImageMemoryBarrier image_memory_barrier(VkImage image, const VkImageSubresourceRange* subresource_range, VkImageLayout old_layout,
                                                        VkImageLayout new_layout, uint32_t src_queue_family_index, uint32_t dst_queue_family_index,
                                                        VkAccessFlags src_access_mask = VK_ACCESS_MEMORY_WRITE_BIT,
                                                        VkAccessFlags dst_access_mask = VK_ACCESS_MEMORY_WRITE_BIT | VK_ACCESS_MEMORY_READ_BIT,
                                                        const void*   pNext           = nullptr);

[[nodiscard]] VkBufferMemoryBarrier buffer_memory_barrier(VkBuffer buffer, uint32_t src_queue_family_index, uint32_t dst_queue_family_index,
                                                          VkAccessFlags src_access_mask = VK_ACCESS_MEMORY_WRITE_BIT,
                                                          VkAccessFlags dst_access_mask = VK_ACCESS_MEMORY_WRITE_BIT | VK_ACCESS_MEMORY_READ_BIT,
                                                          uint64_t offset = 0, uint64_t size = VK_WHOLE_SIZE, const void* pNext = nullptr);

[[nodiscard]] VkMemoryBarrier memory_barrier(VkAccessFlags src_access_mask = VK_ACCESS_MEMORY_WRITE_BIT,
                                             VkAccessFlags dst_access_mask = VK_ACCESS_MEMORY_WRITE_BIT | VK_ACCESS_MEMORY_READ_BIT);

[[nodiscard]] VkEventCreateInfo event_create_info(VkEventCreateFlags flags = 0, const void* pNext = nullptr);
/*
 * CORE EXTENSIONS
 */

// VULKAN 1.2

[[nodiscard]] VkSemaphoreTypeCreateInfoKHR semaphore_type_create_info(VkSemaphoreType type, uint64_t initial_timeline_value = 0,
                                                                      const void* pNext = nullptr);

// VULKAN 1.3

[[nodiscard]] VkImageMemoryBarrier2KHR image_memory_barrier_2(
    VkImage image, VkImageSubresourceRange subresource_range, VkImageLayout old_layout, VkImageLayout new_layout, uint32_t src_queue_family_index,
    uint32_t dst_queue_family_index, VkPipelineStageFlags2 src_stage_mask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT,
    VkPipelineStageFlags2 dst_stage_mask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT, VkAccessFlags2 src_access_mask = VK_ACCESS_2_MEMORY_WRITE_BIT_KHR,
    VkAccessFlags2 dst_access_mask = VK_ACCESS_2_MEMORY_WRITE_BIT_KHR | VK_ACCESS_2_SHADER_READ_BIT_KHR, const void* pNext = nullptr);

[[nodiscard]] VkBufferMemoryBarrier2KHR buffer_memory_barrier_2(VkBuffer buffer, uint32_t src_queue_family_index, uint32_t dst_queue_family_index,
                                                                VkPipelineStageFlags2 src_stage_mask  = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT,
                                                                VkPipelineStageFlags2 dst_stage_mask  = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT,
                                                                VkAccessFlags2KHR     src_access_mask = VK_ACCESS_2_MEMORY_WRITE_BIT_KHR,
                                                                VkAccessFlags2KHR     dst_access_mask = VK_ACCESS_2_MEMORY_WRITE_BIT_KHR |
                                                                                                    VK_ACCESS_2_SHADER_READ_BIT_KHR,
                                                                uint64_t offset = 0, uint64_t size = VK_WHOLE_SIZE, const void* pNext = nullptr);

[[nodiscard]] VkMemoryBarrier2KHR global_memory_barrier_2(VkPipelineStageFlags2 src_stage_mask  = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT,
                                                          VkPipelineStageFlags2 dst_stage_mask  = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT,
                                                          VkAccessFlags2KHR     src_access_mask = VK_ACCESS_2_MEMORY_WRITE_BIT_KHR,
                                                          VkAccessFlags2KHR     dst_access_mask = VK_ACCESS_2_MEMORY_WRITE_BIT_KHR |
                                                                                              VK_ACCESS_2_SHADER_READ_BIT_KHR);

[[nodiscard]] VkDependencyInfoKHR dependency_info_batch(std::span<VkImageMemoryBarrier2KHR>  image_barriers,
                                                        std::span<VkBufferMemoryBarrier2KHR> buffer_barriers,
                                                        std::span<VkMemoryBarrier2KHR> memory_barriers, VkDependencyFlags dependency_flags = 0);

[[nodiscard]] VkDependencyInfoKHR dependency_info(const VkImageMemoryBarrier2KHR* image_barrier, const VkBufferMemoryBarrier2KHR* buffer_barrier,
                                                  const VkMemoryBarrier2KHR* memory_barrier, VkDependencyFlags dependency_flags = 0);

} // namespace vk_lib