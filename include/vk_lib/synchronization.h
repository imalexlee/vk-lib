/*
 * Utilities regarding synchronization primitives and functions
 */

#pragma once
#include <vk_lib/common.h>

// BEGIN SEMAPHORE

VkResult binary_semaphore_create(VkDevice device, VkSemaphore* semaphore, const void* pNext = nullptr);

VkResult timeline_semaphore_create(VkDevice device, uint64_t initial_timeline_value, VkSemaphore* semaphore, const void* pNext = nullptr);

void semaphore_destroy(VkDevice device, VkSemaphore semaphore);

[[nodiscard]] VkSemaphoreSubmitInfoKHR semaphore_submit_info_create(VkSemaphore semaphore, VkPipelineStageFlags2KHR stage_flags,
                                                                    uint64_t timeline_value = 0, uint32_t device_index = 0);

// END SEMAPHORE

// BEGIN FENCE

VkResult fence_create(VkDevice device, VkFenceCreateFlags flags, VkFence* fence, const void* pNext = nullptr);

VkResult fence_batch_reset(VkDevice device, std::span<VkFence> fences);

VkResult fence_reset(VkDevice device, VkFence fence);

void fence_destroy(VkDevice device, VkFence fence);

// END FENCE

// TODO: add events

// BEGIN IMAGE MEMORY BARRIER

[[nodiscard]] VkImageMemoryBarrier
image_memory_barrier_create(VkImage image, const VkImageSubresourceRange* subresource_range, VkImageLayout old_layout, VkImageLayout new_layout,
                            uint32_t src_queue_family_index, uint32_t dst_queue_family_index,
                            VkAccessFlags src_access_flags = VK_ACCESS_SHADER_WRITE_BIT,
                            VkAccessFlags dst_access_flags = VK_ACCESS_SHADER_WRITE_BIT | VK_ACCESS_SHADER_READ_BIT, const void* pNext = nullptr);

[[nodiscard]] VkImageMemoryBarrier2KHR image_memory_barrier_2_create(
    VkImage image, const VkImageSubresourceRange* subresource_range, VkImageLayout old_layout, VkImageLayout new_layout,
    uint32_t src_queue_family_index, uint32_t dst_queue_family_index, VkPipelineStageFlags2 src_stage_flags = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT,
    VkPipelineStageFlags2 dst_stage_flags = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT, VkAccessFlags2 src_access_flags = VK_ACCESS_2_SHADER_WRITE_BIT_KHR,
    VkAccessFlags2 dst_access_flags = VK_ACCESS_2_SHADER_WRITE_BIT_KHR | VK_ACCESS_2_SHADER_READ_BIT_KHR, const void* pNext = nullptr);

// END IMAGE MEMORY BARRIER

// BEGIN BUFFER MEMORY BARRIER

[[nodiscard]] VkBufferMemoryBarrier buffer_memory_barrier_create(VkBuffer buffer, uint32_t src_queue_family_index, uint32_t dst_queue_family_index,
                                                                 VkAccessFlags src_access_flags = VK_ACCESS_SHADER_WRITE_BIT,
                                                                 VkAccessFlags dst_access_flags = VK_ACCESS_SHADER_WRITE_BIT |
                                                                                                  VK_ACCESS_SHADER_READ_BIT,
                                                                 uint64_t offset = 0, uint64_t size = VK_WHOLE_SIZE, const void* pNext = nullptr);

[[nodiscard]] VkBufferMemoryBarrier2KHR
buffer_memory_barrier_2_create(VkBuffer buffer, uint32_t src_queue_family_index, uint32_t dst_queue_family_index,
                               VkPipelineStageFlags2 src_stage_flags  = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT,
                               VkPipelineStageFlags2 dst_stage_flags  = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT,
                               VkAccessFlags2KHR     src_access_flags = VK_ACCESS_2_SHADER_WRITE_BIT_KHR,
                               VkAccessFlags2KHR     dst_access_flags = VK_ACCESS_2_SHADER_WRITE_BIT_KHR | VK_ACCESS_2_SHADER_READ_BIT_KHR,
                               uint64_t offset = 0, uint64_t size = VK_WHOLE_SIZE, const void* pNext = nullptr);

// END BUFFER MEMORY BARRIER

// BEGIN GLOBAL MEMORY BARRIER

[[nodiscard]] VkMemoryBarrier global_memory_barrier_create(VkAccessFlags src_access, VkAccessFlags dst_access);

[[nodiscard]] VkMemoryBarrier2KHR global_memory_barrier_create_2(VkPipelineStageFlags2KHR src_stages, VkAccessFlags2KHR src_access,
                                                                 VkPipelineStageFlags2KHR dst_stages, VkAccessFlags2KHR dst_access);

// END GLOBAL MEMORY BARRIER

// FOR USE WITH VULKAN SYNCHRONIZATION 1

void memory_barrier_insert(VkCommandBuffer command_buffer, VkPipelineStageFlags src_stage_flags, VkPipelineStageFlags dst_stage_flags,
                           std::span<VkImageMemoryBarrier> image_barriers, std::span<VkBufferMemoryBarrier> buffer_barriers,
                           std::span<VkMemoryBarrier> memory_barriers, VkDependencyFlags dependency_flags = 0);

// FOR USE WITH VULKAN SYNCHRONIZATION 2

[[nodiscard]] VkDependencyInfoKHR dependency_info_create(std::span<VkImageMemoryBarrier2KHR>  image_barriers,
                                                         std::span<VkBufferMemoryBarrier2KHR> buffer_barriers,
                                                         std::span<VkMemoryBarrier2KHR> memory_barriers, VkDependencyFlags dependency_flags = 0);

void memory_barrier_insert_2(VkCommandBuffer command_buffer, const VkDependencyInfoKHR* dependency_info);
