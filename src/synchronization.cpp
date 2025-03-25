#include <vk_lib/synchronization.h>

VkResult binary_semaphore_create(VkDevice device, VkSemaphore* semaphore, const void* pNext) {
    VkSemaphoreCreateInfo semaphore_create_info{};
    semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphore_create_info.flags = 0;
    semaphore_create_info.pNext = pNext;

    return vkCreateSemaphore(device, &semaphore_create_info, nullptr, semaphore);
}

VkResult timeline_semaphore_create(VkDevice device, uint64_t initial_timeline_value, VkSemaphore* semaphore, const void* pNext) {
    VkSemaphoreTypeCreateInfoKHR semaphore_type_create_info{};
    semaphore_type_create_info.sType         = VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO_KHR;
    semaphore_type_create_info.semaphoreType = VK_SEMAPHORE_TYPE_TIMELINE;
    semaphore_type_create_info.initialValue  = initial_timeline_value;
    semaphore_type_create_info.pNext         = pNext;

    VkSemaphoreCreateInfo semaphore_create_info{};
    semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphore_create_info.pNext = &semaphore_type_create_info;
    semaphore_create_info.flags = 0;

    return vkCreateSemaphore(device, &semaphore_create_info, nullptr, semaphore);
}

void semaphore_destroy(VkDevice device, VkSemaphore semaphore) { vkDestroySemaphore(device, semaphore, nullptr); }

VkSemaphoreSubmitInfoKHR semaphore_submit_info_create(VkSemaphore semaphore, VkPipelineStageFlags2KHR stage_flags, uint64_t timeline_value,
                                                      uint32_t device_index) {
    VkSemaphoreSubmitInfoKHR semaphore_submit_info{};
    semaphore_submit_info.sType       = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO_KHR;
    semaphore_submit_info.semaphore   = semaphore;
    semaphore_submit_info.stageMask   = stage_flags;
    semaphore_submit_info.deviceIndex = device_index;
    semaphore_submit_info.value       = timeline_value;
    semaphore_submit_info.pNext       = nullptr;

    return semaphore_submit_info;
}

VkResult fence_create(VkDevice device, VkFenceCreateFlags flags, VkFence* fence, const void* pNext) {
    VkFenceCreateInfo fence_create_info{};
    fence_create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_create_info.flags = flags;
    fence_create_info.pNext = pNext;

    return vkCreateFence(device, &fence_create_info, nullptr, fence);
}

VkResult fence_batch_reset(VkDevice device, std::span<VkFence> fences) { return vkResetFences(device, fences.size(), fences.data()); }

VkResult fence_reset(VkDevice device, VkFence fence) { return vkResetFences(device, 1, &fence); }

void fence_destroy(VkDevice device, VkFence fence) { vkDestroyFence(device, fence, nullptr); }

VkImageMemoryBarrier image_memory_barrier_create(VkImage image, const VkImageSubresourceRange* subresource_range, VkImageLayout old_layout,
                                                 VkImageLayout new_layout, uint32_t src_queue_family_index, uint32_t dst_queue_family_index,
                                                 VkAccessFlags src_access_flags, VkAccessFlags dst_access_flags, const void* pNext) {
    VkImageMemoryBarrier image_mem_barrier{};
    image_mem_barrier.sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    image_mem_barrier.srcAccessMask       = src_access_flags;
    image_mem_barrier.dstAccessMask       = dst_access_flags;
    image_mem_barrier.image               = image;
    image_mem_barrier.oldLayout           = old_layout;
    image_mem_barrier.newLayout           = new_layout;
    image_mem_barrier.srcQueueFamilyIndex = src_queue_family_index;
    image_mem_barrier.dstQueueFamilyIndex = dst_queue_family_index;
    image_mem_barrier.subresourceRange    = *subresource_range;
    image_mem_barrier.pNext               = pNext;

    return image_mem_barrier;
}

VkImageMemoryBarrier2KHR image_memory_barrier_2_create(VkImage image, const VkImageSubresourceRange* subresource_range, VkImageLayout old_layout,
                                                       VkImageLayout new_layout, uint32_t src_queue_family_index, uint32_t dst_queue_family_index,
                                                       VkPipelineStageFlags2 src_stage_flags, VkPipelineStageFlags2 dst_stage_flags,
                                                       VkAccessFlags2 src_access_flags, VkAccessFlags2 dst_access_flags, const void* pNext) {
    VkImageMemoryBarrier2KHR image_mem_barrier_2{};
    image_mem_barrier_2.sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2_KHR;
    image_mem_barrier_2.srcStageMask        = src_stage_flags;
    image_mem_barrier_2.srcAccessMask       = src_access_flags;
    image_mem_barrier_2.dstStageMask        = dst_stage_flags;
    image_mem_barrier_2.dstAccessMask       = dst_access_flags;
    image_mem_barrier_2.image               = image;
    image_mem_barrier_2.oldLayout           = old_layout;
    image_mem_barrier_2.newLayout           = new_layout;
    image_mem_barrier_2.srcQueueFamilyIndex = src_queue_family_index;
    image_mem_barrier_2.dstQueueFamilyIndex = dst_queue_family_index;
    image_mem_barrier_2.subresourceRange    = *subresource_range;
    image_mem_barrier_2.pNext               = pNext;

    return image_mem_barrier_2;
}

void buffer_barrier_builder_set_stage_masks_2(BufferBarrierBuilder* builder, VkPipelineStageFlags2KHR src_stages,
                                              VkPipelineStageFlags2KHR dst_stages) {
    builder->src_stage_flags_2 = src_stages;
    builder->dst_stage_flags_2 = dst_stages;
}

void buffer_barrier_builder_set_access_masks_2(BufferBarrierBuilder* builder, VkAccessFlags2KHR src_access, VkAccessFlags2KHR dst_access) {
    builder->src_access_flags_2 = src_access;
    builder->dst_access_flags_2 = dst_access;
}

void buffer_barrier_builder_set_access_masks_1(BufferBarrierBuilder* builder, VkAccessFlags src_access, VkAccessFlags dst_access) {
    builder->buffer_memory_barrier.srcAccessMask = src_access;
    builder->buffer_memory_barrier.dstAccessMask = dst_access;
}

void buffer_barrier_builder_set_queue_family_indices(BufferBarrierBuilder* builder, uint32_t src_queue_family_index,
                                                     uint32_t dst_queue_family_index) {
    builder->buffer_memory_barrier.srcQueueFamilyIndex = src_queue_family_index;
    builder->buffer_memory_barrier.dstQueueFamilyIndex = dst_queue_family_index;
}

void buffer_barrier_builder_set_buffer(BufferBarrierBuilder* builder, VkBuffer buffer, VkDeviceSize offset, VkDeviceSize size) {
    builder->buffer_memory_barrier.buffer = buffer;
    builder->buffer_memory_barrier.offset = offset;
    builder->buffer_memory_barrier.size   = size;
}

void buffer_barrier_builder_set_pNext_chain_1(BufferBarrierBuilder* builder, const void* chain) { builder->buffer_memory_barrier.pNext = chain; }

void buffer_barrier_builder_set_pNext_2(BufferBarrierBuilder* builder, const void* pNext) { builder->pNext_2 = pNext; }

void buffer_barrier_builder_clear(BufferBarrierBuilder* builder) { *builder = BufferBarrierBuilder{}; }

VkBufferMemoryBarrier buffer_barrier_builder_barrier_create(const BufferBarrierBuilder* builder) { return builder->buffer_memory_barrier; }

VkBufferMemoryBarrier2KHR buffer_barrier_builder_barrier_create_2(const BufferBarrierBuilder* builder) {
    const VkBufferMemoryBarrier* buffer_mem_barrier = &builder->buffer_memory_barrier;

    VkBufferMemoryBarrier2KHR buffer_mem_barrier2{};
    buffer_mem_barrier2.sType               = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER_2_KHR;
    buffer_mem_barrier2.pNext               = builder->pNext_2;
    buffer_mem_barrier2.srcStageMask        = builder->src_stage_flags_2;
    buffer_mem_barrier2.srcAccessMask       = builder->src_access_flags_2;
    buffer_mem_barrier2.dstStageMask        = builder->dst_stage_flags_2;
    buffer_mem_barrier2.dstAccessMask       = builder->dst_access_flags_2;
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

VkMemoryBarrier2KHR global_memory_barrier_create_2(VkPipelineStageFlags2KHR src_stages, VkAccessFlags2KHR src_access,
                                                   VkPipelineStageFlags2KHR dst_stages, VkAccessFlags2KHR dst_access) {
    VkMemoryBarrier2KHR memory_barrier{};
    memory_barrier.sType         = VK_STRUCTURE_TYPE_MEMORY_BARRIER_2_KHR;
    memory_barrier.srcStageMask  = src_stages;
    memory_barrier.srcAccessMask = src_access;
    memory_barrier.dstStageMask  = dst_stages;
    memory_barrier.dstAccessMask = dst_access;

    return memory_barrier;
}

void memory_barrier_insert(VkCommandBuffer command_buffer, VkPipelineStageFlags src_stage_flags, VkPipelineStageFlags dst_stage_flags,
                           std::span<VkImageMemoryBarrier> image_barriers, std::span<VkBufferMemoryBarrier> buffer_barriers,
                           std::span<VkMemoryBarrier> memory_barriers, VkDependencyFlags dependency_flags) {
    vkCmdPipelineBarrier(command_buffer, src_stage_flags, dst_stage_flags, dependency_flags, memory_barriers.size(), memory_barriers.data(),
                         buffer_barriers.size(), buffer_barriers.data(), image_barriers.size(), image_barriers.data());
}

VkDependencyInfoKHR dependency_info_create(std::span<VkImageMemoryBarrier2KHR> image_barriers, std::span<VkBufferMemoryBarrier2KHR> buffer_barriers,
                                           std::span<VkMemoryBarrier2KHR> memory_barriers, VkDependencyFlags dependency_flags) {
    VkDependencyInfoKHR dependency_info{};
    dependency_info.sType                    = VK_STRUCTURE_TYPE_DEPENDENCY_INFO_KHR;
    dependency_info.pNext                    = nullptr;
    dependency_info.pImageMemoryBarriers     = image_barriers.data();
    dependency_info.imageMemoryBarrierCount  = image_barriers.size();
    dependency_info.pBufferMemoryBarriers    = buffer_barriers.data();
    dependency_info.bufferMemoryBarrierCount = buffer_barriers.size();
    dependency_info.pMemoryBarriers          = memory_barriers.data();
    dependency_info.memoryBarrierCount       = memory_barriers.size();
    dependency_info.dependencyFlags          = dependency_flags;

    return dependency_info;
}

void memory_barrier_insert_2(VkCommandBuffer command_buffer, const VkDependencyInfoKHR* dependency_info) {
    vkCmdPipelineBarrier2(command_buffer, dependency_info);
}

void memory_barrier_batch_insert_2(VkCommandBuffer command_buffer, std::span<VkImageMemoryBarrier2KHR> image_barriers,
                                   std::span<VkBufferMemoryBarrier2KHR> buffer_barriers, std::span<VkMemoryBarrier2KHR> memory_barriers,
                                   VkDependencyFlags dependency_flags) {
    VkDependencyInfoKHR dependency_info{};
    dependency_info.sType                    = VK_STRUCTURE_TYPE_DEPENDENCY_INFO_KHR;
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

void image_memory_barrier_batch_insert_2(VkCommandBuffer command_buffer, std::span<VkImageMemoryBarrier2KHR> image_barriers,
                                         VkDependencyFlags dependency_flags) {
    VkDependencyInfoKHR dependency_info{};
    dependency_info.sType                   = VK_STRUCTURE_TYPE_DEPENDENCY_INFO_KHR;
    dependency_info.pNext                   = nullptr;
    dependency_info.pImageMemoryBarriers    = image_barriers.data();
    dependency_info.imageMemoryBarrierCount = image_barriers.size();
    dependency_info.dependencyFlags         = dependency_flags;

    vkCmdPipelineBarrier2(command_buffer, &dependency_info);
}

void buffer_barrier_batch_insert_2(VkCommandBuffer command_buffer, std::span<VkBufferMemoryBarrier2KHR> buffer_barriers,
                                   VkDependencyFlags dependency_flags) {
    VkDependencyInfoKHR dependency_info{};
    dependency_info.sType                    = VK_STRUCTURE_TYPE_DEPENDENCY_INFO_KHR;
    dependency_info.pNext                    = nullptr;
    dependency_info.pBufferMemoryBarriers    = buffer_barriers.data();
    dependency_info.bufferMemoryBarrierCount = buffer_barriers.size();
    dependency_info.dependencyFlags          = dependency_flags;

    vkCmdPipelineBarrier2(command_buffer, &dependency_info);
}

void global_memory_barrier_batch_insert_2(VkCommandBuffer command_buffer, std::span<VkMemoryBarrier2KHR> memory_barriers,
                                          VkDependencyFlags dependency_flags) {
    VkDependencyInfoKHR dependency_info{};
    dependency_info.sType              = VK_STRUCTURE_TYPE_DEPENDENCY_INFO_KHR;
    dependency_info.pNext              = nullptr;
    dependency_info.pMemoryBarriers    = memory_barriers.data();
    dependency_info.memoryBarrierCount = memory_barriers.size();
    dependency_info.dependencyFlags    = dependency_flags;

    vkCmdPipelineBarrier2(command_buffer, &dependency_info);
}

void memory_barrier_insert_2(VkCommandBuffer command_buffer, const VkImageMemoryBarrier2KHR* image_barrier,
                             const VkBufferMemoryBarrier2KHR* buffer_barrier, const VkMemoryBarrier2KHR* memory_barrier,
                             VkDependencyFlags dependency_flags) {
    const uint32_t image_barrier_count  = image_barrier == nullptr ? 0 : 1;
    const uint32_t buffer_barrier_count = buffer_barrier == nullptr ? 0 : 1;
    const uint32_t memory_barrier_count = memory_barrier == nullptr ? 0 : 1;

    VkDependencyInfoKHR dependency_info{};
    dependency_info.sType                    = VK_STRUCTURE_TYPE_DEPENDENCY_INFO_KHR;
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

void image_memory_barrier_insert_2(VkCommandBuffer command_buffer, const VkImageMemoryBarrier2KHR* image_barrier,
                                   VkDependencyFlags dependency_flags) {
    VkDependencyInfoKHR dependency_info{};
    dependency_info.sType                    = VK_STRUCTURE_TYPE_DEPENDENCY_INFO_KHR;
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

void buffer_memory_barrier_insert_2(VkCommandBuffer command_buffer, const VkBufferMemoryBarrier2KHR* buffer_barrier,
                                    VkDependencyFlags dependency_flags) {
    VkDependencyInfoKHR dependency_info{};
    dependency_info.sType                    = VK_STRUCTURE_TYPE_DEPENDENCY_INFO_KHR;
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

void global_memory_barrier_insert_2(VkCommandBuffer command_buffer, const VkMemoryBarrier2KHR* memory_barrier, VkDependencyFlags dependency_flags) {
    VkDependencyInfoKHR dependency_info{};
    dependency_info.sType                    = VK_STRUCTURE_TYPE_DEPENDENCY_INFO_KHR;
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
