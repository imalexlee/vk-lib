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

// BEGIN IMAGE MEMORY BARRIER

[[nodiscard]] VkImageMemoryBarrier
image_memory_barrier_create(VkImage image, const VkImageSubresourceRange* subresource_range, VkImageLayout old_layout, VkImageLayout new_layout,
                            uint32_t src_queue_family_index, uint32_t dst_queue_family_index,
                            VkAccessFlags src_access_flags = VK_ACCESS_SHADER_WRITE_BIT,
                            VkAccessFlags dst_access_flags = VK_ACCESS_SHADER_WRITE_BIT | VK_ACCESS_SHADER_READ_BIT, const void* pNext = nullptr);

[[nodiscard]] VkImageMemoryBarrier2KHR image_memory_barrier_2_create(
    VkImage image, const VkImageSubresourceRange* subresource_range, VkImageLayout old_layout, VkImageLayout new_layout,
    uint32_t src_queue_family_index, uint32_t dst_queue_family_index, VkPipelineStageFlags2 src_stage_flags = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT,
    VkPipelineStageFlags2 dst_stage_flags = VK_PIPELINE_STAGE_2_NONE, VkAccessFlags2 src_access_flags = VK_ACCESS_2_SHADER_WRITE_BIT_KHR,
    VkAccessFlags2 dst_access_flags = VK_ACCESS_2_SHADER_WRITE_BIT_KHR | VK_ACCESS_2_SHADER_READ_BIT_KHR, const void* pNext = nullptr);

// END IMAGE MEMORY BARRIER

// BEGIN BUFFER MEMORY BARRIER

struct BufferBarrierBuilder {
    VkBufferMemoryBarrier    buffer_memory_barrier{VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER};
    VkPipelineStageFlags2KHR src_stage_flags_2{};
    VkPipelineStageFlags2KHR dst_stage_flags_2{};
    VkAccessFlags2           src_access_flags_2{};
    VkAccessFlags2           dst_access_flags_2{};
    const void*              pNext_2{};
};

void buffer_barrier_builder_set_stage_masks_2(BufferBarrierBuilder* builder, VkPipelineStageFlags2KHR src_stages,
                                              VkPipelineStageFlags2KHR dst_stages);

void buffer_barrier_builder_set_access_masks_2(BufferBarrierBuilder* builder, VkAccessFlags2KHR src_access, VkAccessFlags2KHR dst_access);

void buffer_barrier_builder_set_access_masks_1(BufferBarrierBuilder* builder, VkAccessFlags src_access, VkAccessFlags dst_access);

void buffer_barrier_builder_set_queue_family_indices(BufferBarrierBuilder* builder, uint32_t src_queue_family_index, uint32_t dst_queue_family_index);

void buffer_barrier_builder_set_buffer(BufferBarrierBuilder* builder, VkBuffer buffer, VkDeviceSize offset, VkDeviceSize size);

void buffer_barrier_builder_set_pNext_chain_1(BufferBarrierBuilder* builder, const void* chain);

void buffer_barrier_builder_set_pNext_2(BufferBarrierBuilder* builder, const void* pNext);

void buffer_barrier_builder_clear(BufferBarrierBuilder* builder);

[[nodiscard]] VkBufferMemoryBarrier buffer_barrier_builder_barrier_create(const BufferBarrierBuilder* builder);

[[nodiscard]] VkBufferMemoryBarrier2KHR buffer_barrier_builder_barrier_create_2(const BufferBarrierBuilder* builder);

// TODO: add singular struct function for buffer barrier creations

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

VkDependencyInfoKHR dependency_info_create(std::span<VkImageMemoryBarrier2KHR> image_barriers, std::span<VkBufferMemoryBarrier2KHR> buffer_barriers,
                                           std::span<VkMemoryBarrier2KHR> memory_barriers, VkDependencyFlags dependency_flags = 0);

void memory_barrier_insert_2(VkCommandBuffer command_buffer, const VkDependencyInfoKHR* dependency_info);
