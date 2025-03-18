#include <vk_lib/synchronization.h>

VkResult binary_semaphore_create(VkDevice device, VkSemaphore* semaphore) {
    VkSemaphoreCreateInfo semaphore_create_info{};
    semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphore_create_info.flags = 0;

    return vkCreateSemaphore(device, &semaphore_create_info, nullptr, semaphore);

}

VkResult timeline_semaphore_create(VkDevice device, uint64_t initial_timeline_value, VkSemaphore* semaphore) {
    VkSemaphoreTypeCreateInfo semaphore_type_create_info{};
    semaphore_type_create_info.sType         = VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO;
    semaphore_type_create_info.semaphoreType = VK_SEMAPHORE_TYPE_TIMELINE;
    semaphore_type_create_info.initialValue  = initial_timeline_value;

    VkSemaphoreCreateInfo semaphore_create_info{};
    semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphore_create_info.pNext = &semaphore_type_create_info;
    semaphore_create_info.flags = 0;

    return vkCreateSemaphore(device, &semaphore_create_info, nullptr, semaphore);
}

VkSemaphoreSubmitInfo binary_semaphore_submit_info_create(VkSemaphore semaphore, VkPipelineStageFlags2 stage_flags, uint32_t device_index) {
    VkSemaphoreSubmitInfo semaphore_submit_info{};
    semaphore_submit_info.sType       = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO;
    semaphore_submit_info.semaphore   = semaphore;
    semaphore_submit_info.stageMask   = stage_flags;
    semaphore_submit_info.deviceIndex = device_index;
    semaphore_submit_info.pNext       = nullptr;

    return semaphore_submit_info;
}

VkSemaphoreSubmitInfo timeline_semaphore_submit_info_create(VkSemaphore semaphore, uint32_t timeline_value, VkPipelineStageFlags2 stage_flags,
                                                            uint32_t device_index) {
    VkSemaphoreSubmitInfo semaphore_submit_info{};
    semaphore_submit_info.sType       = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO;
    semaphore_submit_info.semaphore   = semaphore;
    semaphore_submit_info.stageMask   = stage_flags;
    semaphore_submit_info.deviceIndex = device_index;
    semaphore_submit_info.value       = timeline_value;
    semaphore_submit_info.pNext       = nullptr;

    return semaphore_submit_info;
}

VkResult fence_create(VkDevice device, VkFenceCreateFlags flags, VkFence* fence) {
    VkFenceCreateInfo fence_create_info{};
    fence_create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_create_info.flags = flags;
    fence_create_info.pNext = nullptr;

    return vkCreateFence(device, &fence_create_info, nullptr, fence);
}


void image_barrier_builder_set_stage_masks2(ImageBarrierBuilder* builder, VkPipelineStageFlags2 src_stages, VkPipelineStageFlags2 dst_stages) {
    builder->src_stage_flags = src_stages;
    builder->dst_stage_flags = dst_stages;
}

void image_barrier_builder_set_access_masks(ImageBarrierBuilder* builder, VkAccessFlags src_access, VkAccessFlags dst_access) {
    builder->image_memory_barrier.srcAccessMask = src_access;
    builder->image_memory_barrier.dstAccessMask = dst_access;
}

void image_barrier_builder_set_access_masks2(ImageBarrierBuilder* builder, VkAccessFlags2 src_access, VkAccessFlags2 dst_access) {
    builder->src_access_flags2 = src_access;
    builder->dst_access_flags2 = dst_access;
}

void image_barrier_builder_set_layouts(ImageBarrierBuilder* builder, VkImageLayout old_layout, VkImageLayout new_layout) {
    builder->image_memory_barrier.oldLayout = old_layout;
    builder->image_memory_barrier.newLayout = new_layout;
}

void image_barrier_builder_set_image(ImageBarrierBuilder* builder, VkImage image, VkImageSubresourceRange subresource_range) {
    builder->image_memory_barrier.image            = image;
    builder->image_memory_barrier.subresourceRange = subresource_range;
}

void image_barrier_builder_set_queue_family_indices(ImageBarrierBuilder* builder, uint32_t src_queue_family_index, uint32_t dst_queue_family_index) {
    builder->image_memory_barrier.srcQueueFamilyIndex = src_queue_family_index;
    builder->image_memory_barrier.dstQueueFamilyIndex = dst_queue_family_index;
}

VkImageMemoryBarrier image_barrier_builder_barrier_create(const ImageBarrierBuilder* builder) {
    return builder->image_memory_barrier;
}

VkImageMemoryBarrier2 image_barrier_builder_barrier_create2(const ImageBarrierBuilder* builder) {
    const VkImageMemoryBarrier* image_mem_barrier = &builder->image_memory_barrier;

    VkImageMemoryBarrier2 image_mem_barrier2{};
    image_mem_barrier2.sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2;
    image_mem_barrier2.pNext               = nullptr;
    image_mem_barrier2.image               = image_mem_barrier->image;
    image_mem_barrier2.srcStageMask        = builder->src_stage_flags;
    image_mem_barrier2.srcAccessMask       = builder->src_access_flags2;
    image_mem_barrier2.dstStageMask        = builder->dst_stage_flags;
    image_mem_barrier2.dstAccessMask       = builder->dst_access_flags2;
    image_mem_barrier2.oldLayout           = image_mem_barrier->oldLayout;
    image_mem_barrier2.newLayout           = image_mem_barrier->newLayout;
    image_mem_barrier2.srcQueueFamilyIndex = image_mem_barrier->srcQueueFamilyIndex;
    image_mem_barrier2.dstQueueFamilyIndex = image_mem_barrier->dstQueueFamilyIndex;

    return image_mem_barrier2;
}

void buffer_barrier_builder_set_stage_masks2(BufferBarrierBuilder* builder, VkPipelineStageFlags2 src_stages, VkPipelineStageFlags2 dst_stages) {
    builder->src_stage_flags = src_stages;
    builder->dst_stage_flags = dst_stages;
}


void buffer_barrier_builder_set_access_masks2(BufferBarrierBuilder* builder,
                                              VkAccessFlags2 src_access, VkAccessFlags2 dst_access) {
    builder->src_access_flags2 = src_access;
    builder->dst_access_flags2 = dst_access;
}

void buffer_barrier_builder_set_access_masks(BufferBarrierBuilder* builder, VkAccessFlags src_access, VkAccessFlags dst_access) {
    builder->buffer_memory_barrier.srcAccessMask = src_access;
    builder->buffer_memory_barrier.dstAccessMask = dst_access;
}

void buffer_barrier_builder_set_queue_family_indices(BufferBarrierBuilder* builder, uint32_t src_queue_family_index, uint32_t dst_queue_family_index) {
    builder->buffer_memory_barrier.srcQueueFamilyIndex = src_queue_family_index;
    builder->buffer_memory_barrier.dstQueueFamilyIndex = dst_queue_family_index;
}

void buffer_barrier_builder_set_buffer(BufferBarrierBuilder* builder, VkBuffer buffer, VkDeviceSize offset, VkDeviceSize size) {
    builder->buffer_memory_barrier.buffer = buffer;
    builder->buffer_memory_barrier.offset = offset;
    builder->buffer_memory_barrier.size   = size;
}

VkBufferMemoryBarrier buffer_barrier_builder_barrier_create(const BufferBarrierBuilder* builder) {
    return builder->buffer_memory_barrier;
}

VkBufferMemoryBarrier2 buffer_barrier_builder_barrier_create2(const BufferBarrierBuilder* builder) {
    const VkBufferMemoryBarrier* buffer_mem_barrier = &builder->buffer_memory_barrier;

    VkBufferMemoryBarrier2 buffer_mem_barrier2{};
    buffer_mem_barrier2.sType               = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER_2;
    buffer_mem_barrier2.pNext               = nullptr;
    buffer_mem_barrier2.srcStageMask        = builder->src_stage_flags;
    buffer_mem_barrier2.srcAccessMask       = builder->src_access_flags2;
    buffer_mem_barrier2.dstStageMask        = builder->dst_stage_flags;
    buffer_mem_barrier2.dstAccessMask       = builder->dst_access_flags2;
    buffer_mem_barrier2.buffer              = buffer_mem_barrier->buffer;
    buffer_mem_barrier2.offset              = buffer_mem_barrier->offset;
    buffer_mem_barrier2.size                = buffer_mem_barrier->size;
    buffer_mem_barrier2.srcQueueFamilyIndex = buffer_mem_barrier->srcQueueFamilyIndex;
    buffer_mem_barrier2.dstQueueFamilyIndex = buffer_mem_barrier->dstQueueFamilyIndex;

    return buffer_mem_barrier2;
}

VkMemoryBarrier global_memory_barrier_create(VkAccessFlags src_access, VkAccessFlags dst_access) {
    VkMemoryBarrier memory_barrier{};
    memory_barrier.sType         = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
    memory_barrier.srcAccessMask = src_access;
    memory_barrier.dstAccessMask = dst_access;

    return memory_barrier;
}

VkMemoryBarrier2 global_memory_barrier_create2(VkPipelineStageFlags2 src_stages, VkAccessFlags2 src_access, VkPipelineStageFlags2 dst_stages, VkAccessFlags2 dst_access) {
    VkMemoryBarrier2 memory_barrier{};
    memory_barrier.sType         = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
    memory_barrier.srcStageMask  = src_stages;
    memory_barrier.srcAccessMask = src_access;
    memory_barrier.dstStageMask  = dst_stages;
    memory_barrier.dstAccessMask = dst_access;

    return memory_barrier;
}

void memory_barrier_multi_insert(VkCommandBuffer command_buffer, VkPipelineStageFlags src_stage_flags, VkPipelineStageFlags dst_stage_flags,
                                 std::span<VkImageMemoryBarrier> image_barriers,
                                 std::span<VkBufferMemoryBarrier> buffer_barriers,
                                 std::span<VkMemoryBarrier> memory_barriers, VkDependencyFlags dependency_flags) {
    vkCmdPipelineBarrier(command_buffer, src_stage_flags, dst_stage_flags, dependency_flags, memory_barriers.size(), memory_barriers.data(), buffer_barriers.size(), buffer_barriers.data(),
                         image_barriers.size(), image_barriers.data());
}

void image_memory_barrier_multi_insert(VkCommandBuffer command_buffer, VkPipelineStageFlags src_stage_flags, VkPipelineStageFlags dst_stage_flags,
                                       std::span<VkImageMemoryBarrier> image_barriers, VkDependencyFlags dependency_flags) {
    vkCmdPipelineBarrier(command_buffer, src_stage_flags, dst_stage_flags, dependency_flags, 0, nullptr, 0, nullptr,
                         image_barriers.size(), image_barriers.data());
}

void buffer_memory_barrier_multi_insert(VkCommandBuffer command_buffer, VkPipelineStageFlags src_stage_flags, VkPipelineStageFlags dst_stage_flags,
                                        std::span<VkBufferMemoryBarrier> buffer_barriers, VkDependencyFlags dependency_flags) {
    vkCmdPipelineBarrier(command_buffer, src_stage_flags, dst_stage_flags, dependency_flags, 0, nullptr, buffer_barriers.size(), buffer_barriers.data(),
                         0, nullptr);
}

void global_memory_barrier_multi_insert(VkCommandBuffer command_buffer, VkPipelineStageFlags src_stage_flags, VkPipelineStageFlags dst_stage_flags,
                                        std::span<VkMemoryBarrier> memory_barriers, VkDependencyFlags dependency_flags) {
    vkCmdPipelineBarrier(command_buffer, src_stage_flags, dst_stage_flags, dependency_flags, memory_barriers.size(), memory_barriers.data(), 0, nullptr,
                         0, nullptr);
}

void memory_barrier_insert(VkCommandBuffer command_buffer, VkPipelineStageFlags src_stage_flags, VkPipelineStageFlags dst_stage_flags,
                           const VkImageMemoryBarrier* image_barrier,
                           const VkBufferMemoryBarrier* buffer_barrier,
                           const VkMemoryBarrier* memory_barrier, VkDependencyFlags dependency_flags) {
    const uint32_t image_barrier_count  = image_barrier == nullptr ? 0 : 1;
    const uint32_t buffer_barrier_count = buffer_barrier == nullptr ? 0 : 1;
    const uint32_t memory_barrier_count = memory_barrier == nullptr ? 0 : 1;

    vkCmdPipelineBarrier(command_buffer, src_stage_flags, dst_stage_flags, dependency_flags, memory_barrier_count, memory_barrier, buffer_barrier_count, buffer_barrier,
                         image_barrier_count, image_barrier);
}

void image_barrier_insert(VkCommandBuffer command_buffer, VkPipelineStageFlags src_stage_flags, VkPipelineStageFlags dst_stage_flags,
                          const VkImageMemoryBarrier* image_barrier, VkDependencyFlags dependency_flags) {
    vkCmdPipelineBarrier(command_buffer, src_stage_flags, dst_stage_flags, dependency_flags, 0, nullptr, 0, nullptr,
                         1, image_barrier);
}

void buffer_barrier_insert(VkCommandBuffer command_buffer, VkPipelineStageFlags src_stage_flags, VkPipelineStageFlags dst_stage_flags,
                           const VkBufferMemoryBarrier* buffer_barrier,
                           VkDependencyFlags dependency_flags) {
    vkCmdPipelineBarrier(command_buffer, src_stage_flags, dst_stage_flags, dependency_flags, 0, nullptr, 1, buffer_barrier,
                         0, nullptr);
}

void global_memory_barrier_insert(VkCommandBuffer command_buffer, VkPipelineStageFlags src_stage_flags, VkPipelineStageFlags dst_stage_flags,
                                  const VkMemoryBarrier* memory_barrier, VkDependencyFlags dependency_flags) {
    vkCmdPipelineBarrier(command_buffer, src_stage_flags, dst_stage_flags, dependency_flags, 1, memory_barrier, 0, nullptr,
                         0, nullptr);
}

void memory_barrier_multi_insert2(VkCommandBuffer command_buffer, std::span<VkImageMemoryBarrier2> image_barriers, std::span<VkBufferMemoryBarrier2> buffer_barriers,
                                  std::span<VkMemoryBarrier2> memory_barriers, VkDependencyFlags dependency_flags) {
    VkDependencyInfo dependency_info{};
    dependency_info.sType                    = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
    dependency_info.pNext                    = nullptr;
    dependency_info.pImageMemoryBarriers     = image_barriers.data();
    dependency_info.imageMemoryBarrierCount  = image_barriers.size();
    dependency_info.pBufferMemoryBarriers    = buffer_barriers.data();
    dependency_info.bufferMemoryBarrierCount = buffer_barriers.size();
    dependency_info.pMemoryBarriers          = memory_barriers.data();
    dependency_info.memoryBarrierCount       = memory_barriers.size();
    dependency_info.dependencyFlags          = dependency_flags;

    vkCmdPipelineBarrier2(command_buffer, &dependency_info);
}

void image_memory_barrier_multi_insert2(VkCommandBuffer command_buffer, std::span<VkImageMemoryBarrier2> image_barriers, VkDependencyFlags dependency_flags) {
    VkDependencyInfo dependency_info{};
    dependency_info.sType                   = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
    dependency_info.pNext                   = nullptr;
    dependency_info.pImageMemoryBarriers    = image_barriers.data();
    dependency_info.imageMemoryBarrierCount = image_barriers.size();
    dependency_info.dependencyFlags         = dependency_flags;

    vkCmdPipelineBarrier2(command_buffer, &dependency_info);
}

void buffer_barrier_multi_insert2(VkCommandBuffer command_buffer, std::span<VkBufferMemoryBarrier2> buffer_barriers, VkDependencyFlags dependency_flags) {
    VkDependencyInfo dependency_info{};
    dependency_info.sType                    = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
    dependency_info.pNext                    = nullptr;
    dependency_info.pBufferMemoryBarriers    = buffer_barriers.data();
    dependency_info.bufferMemoryBarrierCount = buffer_barriers.size();
    dependency_info.dependencyFlags          = dependency_flags;

    vkCmdPipelineBarrier2(command_buffer, &dependency_info);
}

void global_memory_barrier_multi_insert2(VkCommandBuffer command_buffer, std::span<VkMemoryBarrier2> memory_barriers, VkDependencyFlags dependency_flags) {
    VkDependencyInfo dependency_info{};
    dependency_info.sType              = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
    dependency_info.pNext              = nullptr;
    dependency_info.pMemoryBarriers    = memory_barriers.data();
    dependency_info.memoryBarrierCount = memory_barriers.size();
    dependency_info.dependencyFlags    = dependency_flags;

    vkCmdPipelineBarrier2(command_buffer, &dependency_info);
}

void memory_barrier_insert2(VkCommandBuffer command_buffer, const VkImageMemoryBarrier2* image_barrier, const VkBufferMemoryBarrier2* buffer_barrier,
                            const VkMemoryBarrier2* memory_barrier, VkDependencyFlags dependency_flags) {
    const uint32_t image_barrier_count  = image_barrier == nullptr ? 0 : 1;
    const uint32_t buffer_barrier_count = buffer_barrier == nullptr ? 0 : 1;
    const uint32_t memory_barrier_count = memory_barrier == nullptr ? 0 : 1;

    VkDependencyInfo dependency_info{};
    dependency_info.sType                    = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
    dependency_info.pNext                    = nullptr;
    dependency_info.pImageMemoryBarriers     = image_barrier;
    dependency_info.imageMemoryBarrierCount  = image_barrier_count;
    dependency_info.pBufferMemoryBarriers    = buffer_barrier;
    dependency_info.bufferMemoryBarrierCount = buffer_barrier_count;
    dependency_info.pMemoryBarriers          = memory_barrier;
    dependency_info.memoryBarrierCount       = memory_barrier_count;
    dependency_info.dependencyFlags          = dependency_flags;

    vkCmdPipelineBarrier2(command_buffer, &dependency_info);
}

void image_memory_barrier_insert2(VkCommandBuffer command_buffer, const VkImageMemoryBarrier2* image_barrier, VkDependencyFlags dependency_flags) {

    VkDependencyInfo dependency_info{};
    dependency_info.sType                    = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
    dependency_info.pNext                    = nullptr;
    dependency_info.pImageMemoryBarriers     = image_barrier;
    dependency_info.imageMemoryBarrierCount  = 1;
    dependency_info.pBufferMemoryBarriers    = nullptr;
    dependency_info.bufferMemoryBarrierCount = 0;
    dependency_info.pMemoryBarriers          = nullptr;
    dependency_info.memoryBarrierCount       = 0;
    dependency_info.dependencyFlags          = dependency_flags;

    vkCmdPipelineBarrier2(command_buffer, &dependency_info);
}

void buffer_memory_barrier_insert2(VkCommandBuffer command_buffer, const VkBufferMemoryBarrier2* buffer_barrier,
                                   VkDependencyFlags dependency_flags) {

    VkDependencyInfo dependency_info{};
    dependency_info.sType                    = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
    dependency_info.pNext                    = nullptr;
    dependency_info.pImageMemoryBarriers     = nullptr;
    dependency_info.imageMemoryBarrierCount  = 0;
    dependency_info.pBufferMemoryBarriers    = buffer_barrier;
    dependency_info.bufferMemoryBarrierCount = 1;
    dependency_info.pMemoryBarriers          = nullptr;
    dependency_info.memoryBarrierCount       = 0;
    dependency_info.dependencyFlags          = dependency_flags;

    vkCmdPipelineBarrier2(command_buffer, &dependency_info);
}

void global_memory_barrier_insert2(VkCommandBuffer command_buffer, const VkMemoryBarrier2* memory_barrier, VkDependencyFlags dependency_flags) {

    VkDependencyInfo dependency_info{};
    dependency_info.sType                    = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
    dependency_info.pNext                    = nullptr;
    dependency_info.pImageMemoryBarriers     = nullptr;
    dependency_info.imageMemoryBarrierCount  = 0;
    dependency_info.pBufferMemoryBarriers    = nullptr;
    dependency_info.bufferMemoryBarrierCount = 0;
    dependency_info.pMemoryBarriers          = memory_barrier;
    dependency_info.memoryBarrierCount       = 1;
    dependency_info.dependencyFlags          = dependency_flags;

    vkCmdPipelineBarrier2(command_buffer, &dependency_info);
}
