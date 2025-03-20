/*
* Utilities regarding synchronization primitives and functions
*/

#pragma once
#include <vk_lib/common.h>

// BEGIN SEMAPHORE

VkResult binary_semaphore_create(VkDevice device, VkSemaphore* semaphore,
                                 const void* pNext = nullptr);

VkResult timeline_semaphore_create(VkDevice device, uint64_t initial_timeline_value,
                                   VkSemaphore* semaphore, const void* pNext = nullptr);

void semaphore_destroy(VkDevice device, VkSemaphore semaphore);

[[nodiscard]] VkSemaphoreSubmitInfo binary_semaphore_submit_info_create(VkSemaphore semaphore,
    VkPipelineStageFlags2 stage_flags,
    uint32_t device_index = 0);

[[nodiscard]] VkSemaphoreSubmitInfo timeline_semaphore_submit_info_create(VkSemaphore semaphore,
    uint32_t timeline_value,
    VkPipelineStageFlags2 stage_flags,
    uint32_t device_index = 0);

// END SEMAPHORE

// BEGIN FENCE

VkResult fence_create(VkDevice device, VkFenceCreateFlags flags, VkFence* fence,
                      const void* pNext = nullptr);

void fence_destroy(VkDevice device, VkFence fence);

// END FENCE

// BEGIN IMAGE MEMORY BARRIER

struct ImageBarrierBuilder
{
    VkImageMemoryBarrier image_memory_barrier{VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
    VkPipelineStageFlags2 src_stage_flags{};
    VkPipelineStageFlags2 dst_stage_flags{};
    VkAccessFlags2 src_access_flags_2{};
    VkAccessFlags2 dst_access_flags_2{};
    const void* pNext_2{};
};

void image_barrier_builder_set_stage_masks2(ImageBarrierBuilder* builder,
                                            VkPipelineStageFlags2 src_stages,
                                            VkPipelineStageFlags2 dst_stages);

void image_barrier_builder_set_access_masks2(ImageBarrierBuilder* builder,
                                             VkAccessFlags2 src_access,
                                             VkAccessFlags2 dst_access);

void image_barrier_builder_set_access_masks(ImageBarrierBuilder* builder,
                                            VkAccessFlags src_access,
                                            VkAccessFlags dst_access);

void image_barrier_builder_set_layouts(ImageBarrierBuilder* builder, VkImageLayout old_layout,
                                       VkImageLayout new_layout);

void image_barrier_builder_set_image(ImageBarrierBuilder* builder, VkImage image,
                                     VkImageSubresourceRange subresource_range);

void image_barrier_builder_set_queue_family_indices(ImageBarrierBuilder* builder,
                                                    uint32_t src_queue_family_index,
                                                    uint32_t dst_queue_family_index);

void image_barrier_builder_set_pNext_1(ImageBarrierBuilder* builder, const void* pNext);

void image_barrier_builder_set_pNext_2(ImageBarrierBuilder* builder, const void* pNext);

void image_barrier_builder_clear(ImageBarrierBuilder* builder);

[[nodiscard]] VkImageMemoryBarrier image_barrier_builder_barrier_create(
    const ImageBarrierBuilder* builder);

[[nodiscard]] VkImageMemoryBarrier2 image_barrier_builder_barrier_create_2(
    const ImageBarrierBuilder* builder);

// END IMAGE MEMORY BARRIER

// BEGIN BUFFER MEMORY BARRIER

struct BufferBarrierBuilder
{
    VkBufferMemoryBarrier buffer_memory_barrier{VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER};
    VkPipelineStageFlags2 src_stage_flags{};
    VkPipelineStageFlags2 dst_stage_flags{};
    VkAccessFlags2 src_access_flags_2{};
    VkAccessFlags2 dst_access_flags_2{};
    const void* pNext_2{};
};

void buffer_barrier_builder_set_stage_masks_2(BufferBarrierBuilder* builder,
                                              VkPipelineStageFlags2 src_stages,
                                              VkPipelineStageFlags2 dst_stages);

void buffer_barrier_builder_set_access_masks_2(BufferBarrierBuilder* builder,
                                               VkAccessFlags2 src_access,
                                               VkAccessFlags2 dst_access);

void buffer_barrier_builder_set_access_masks_1(BufferBarrierBuilder* builder,
                                               VkAccessFlags src_access, VkAccessFlags dst_access);

void buffer_barrier_builder_set_queue_family_indices(BufferBarrierBuilder* builder,
                                                     uint32_t src_queue_family_index,
                                                     uint32_t dst_queue_family_index);

void buffer_barrier_builder_set_buffer(BufferBarrierBuilder* builder, VkBuffer buffer,
                                       VkDeviceSize offset, VkDeviceSize size);

void buffer_barrier_builder_set_pNext_1(BufferBarrierBuilder* builder, const void* pNext);

void buffer_barrier_builder_set_pNext_2(BufferBarrierBuilder* builder, const void* pNext);

void buffer_barrier_builder_clear(BufferBarrierBuilder* builder);

[[nodiscard]] VkBufferMemoryBarrier buffer_barrier_builder_barrier_create(
    const BufferBarrierBuilder* builder);

[[nodiscard]] VkBufferMemoryBarrier2 buffer_barrier_builder_barrier_create_2(
    const BufferBarrierBuilder* builder);


// END BUFFER MEMORY BARRIER

// BEGIN GLOBAL MEMORY BARRIER

[[nodiscard]] VkMemoryBarrier global_memory_barrier_create();

[[nodiscard]] VkMemoryBarrier2 global_memory_barrier_create_2();

// END GLOBAL MEMORY BARRIER


// FOR USE WITH VULKAN SYNCHRONIZATION 1

void memory_barrier_multi_insert(VkCommandBuffer command_buffer,
                                 VkPipelineStageFlags src_stage_flags,
                                 VkPipelineStageFlags dst_stage_flags,
                                 std::span<VkImageMemoryBarrier> image_barriers,
                                 std::span<VkBufferMemoryBarrier> buffer_barriers,
                                 std::span<VkMemoryBarrier> memory_barriers,
                                 VkDependencyFlags dependency_flags = 0);

void image_memory_barrier_multi_insert(VkCommandBuffer command_buffer,
                                       VkPipelineStageFlags src_stage_flags,
                                       VkPipelineStageFlags dst_stage_flags,
                                       std::span<VkImageMemoryBarrier> image_barriers,
                                       VkDependencyFlags dependency_flags = 0);

void buffer_memory_barrier_multi_insert(VkCommandBuffer command_buffer,
                                        VkPipelineStageFlags src_stage_flags,
                                        VkPipelineStageFlags dst_stage_flags,
                                        std::span<VkBufferMemoryBarrier> buffer_barriers,
                                        VkDependencyFlags dependency_flags = 0);

void global_memory_barrier_multi_insert(VkCommandBuffer command_buffer,
                                        VkPipelineStageFlags src_stage_flags,
                                        VkPipelineStageFlags dst_stage_flags,
                                        std::span<VkMemoryBarrier> memory_barriers,
                                        VkDependencyFlags dependency_flags = 0);


void memory_barrier_insert(VkCommandBuffer command_buffer, VkPipelineStageFlags src_stage_flags,
                           VkPipelineStageFlags dst_stage_flags,
                           const VkImageMemoryBarrier* image_barrier,
                           const VkBufferMemoryBarrier* buffer_barrier,
                           const VkMemoryBarrier* memory_barrier,
                           VkDependencyFlags dependency_flags = 0);

void image_barrier_insert(VkCommandBuffer command_buffer, VkPipelineStageFlags src_stage_flags,
                          VkPipelineStageFlags dst_stage_flags,
                          const VkImageMemoryBarrier* image_barrier,
                          VkDependencyFlags dependency_flags = 0);

void buffer_barrier_insert(VkCommandBuffer command_buffer, VkPipelineStageFlags src_stage_flags,
                           VkPipelineStageFlags dst_stage_flags,
                           const VkBufferMemoryBarrier* buffer_barrier,
                           VkDependencyFlags dependency_flags = 0);

void global_memory_barrier_insert(VkCommandBuffer command_buffer,
                                  VkPipelineStageFlags src_stage_flags,
                                  VkPipelineStageFlags dst_stage_flags,
                                  const VkMemoryBarrier* memory_barrier,
                                  VkDependencyFlags dependency_flags = 0);

// FOR USE WITH VULKAN SYNCHRONIZATION 2

void memory_barrier_multi_insert_2(VkCommandBuffer command_buffer,
                                   std::span<VkImageMemoryBarrier2> image_barriers,
                                   std::span<VkBufferMemoryBarrier2> buffer_barriers,
                                   std::span<VkMemoryBarrier2> memory_barriers,
                                   VkDependencyFlags dependency_flags = 0);

void image_memory_barrier_multi_insert_2(VkCommandBuffer command_buffer,
                                         std::span<VkImageMemoryBarrier2> image_barriers,
                                         VkDependencyFlags dependency_flags = 0);

void buffer_barrier_multi_insert_2(VkCommandBuffer command_buffer,
                                   std::span<VkBufferMemoryBarrier2> buffer_barriers,
                                   VkDependencyFlags dependency_flags = 0);

void global_memory_barrier_multi_insert_2(VkCommandBuffer command_buffer,
                                          std::span<VkMemoryBarrier2> memory_barriers,
                                          VkDependencyFlags dependency_flags = 0);


void memory_barrier_insert_2(VkCommandBuffer command_buffer,
                             const VkImageMemoryBarrier2* image_barrier,
                             const VkBufferMemoryBarrier2* buffer_barrier,
                             const VkMemoryBarrier2* memory_barrier,
                             VkDependencyFlags dependency_flags = 0);

void image_memory_barrier_insert_2(VkCommandBuffer command_buffer,
                                   const VkImageMemoryBarrier2* image_barrier,
                                   VkDependencyFlags dependency_flags = 0);

void buffer_memory_barrier_insert_2(VkCommandBuffer command_buffer,
                                    const VkBufferMemoryBarrier2* buffer_barrier,
                                    VkDependencyFlags dependency_flags = 0);

void global_memory_barrier_insert_2(VkCommandBuffer command_buffer,
                                    const VkMemoryBarrier2* memory_barrier,
                                    VkDependencyFlags dependency_flags = 0);
