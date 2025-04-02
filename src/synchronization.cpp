#include <vk_lib/synchronization.h>

namespace vk_lib {

VkSemaphoreCreateInfo semaphore_create_info(const void* pNext) {
    VkSemaphoreCreateInfo semaphore_create_info{};
    semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphore_create_info.flags = 0;
    semaphore_create_info.pNext = pNext;

    return semaphore_create_info;
}

VkSemaphoreSubmitInfoKHR semaphore_submit_info(VkSemaphore semaphore, VkPipelineStageFlags2KHR stage_flags, uint64_t timeline_value,
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

VkFenceCreateInfo fence_create_info(VkFenceCreateFlags flags, const void* pNext) {
    VkFenceCreateInfo fence_create_info{};
    fence_create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_create_info.flags = flags;
    fence_create_info.pNext = pNext;

    return fence_create_info;
}

//--------------------------------------------------------------------------------------------------------------------------------//

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

VkBufferMemoryBarrier buffer_memory_barrier_create(VkBuffer buffer, uint32_t src_queue_family_index, uint32_t dst_queue_family_index,
                                                   VkAccessFlags src_access_flags, VkAccessFlags dst_access_flags, uint64_t offset, uint64_t size,
                                                   const void* pNext) {
    VkBufferMemoryBarrier buffer_memory_barrier{};
    buffer_memory_barrier.sType               = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
    buffer_memory_barrier.srcAccessMask       = src_access_flags;
    buffer_memory_barrier.dstAccessMask       = dst_access_flags;
    buffer_memory_barrier.srcQueueFamilyIndex = src_queue_family_index;
    buffer_memory_barrier.dstQueueFamilyIndex = dst_queue_family_index;
    buffer_memory_barrier.buffer              = buffer;
    buffer_memory_barrier.offset              = offset;
    buffer_memory_barrier.size                = size;
    buffer_memory_barrier.pNext               = pNext;

    return buffer_memory_barrier;
}

VkMemoryBarrier global_memory_barrier_create(VkAccessFlags src_access, VkAccessFlags dst_access) {
    VkMemoryBarrier memory_barrier{};
    memory_barrier.sType         = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
    memory_barrier.srcAccessMask = src_access;
    memory_barrier.dstAccessMask = dst_access;

    return memory_barrier;
}

void pipeline_barrier_insert(VkCommandBuffer command_buffer, VkPipelineStageFlags src_stage_flags, VkPipelineStageFlags dst_stage_flags,
                             std::span<VkImageMemoryBarrier> image_barriers, std::span<VkBufferMemoryBarrier> buffer_barriers,
                             std::span<VkMemoryBarrier> memory_barriers, VkDependencyFlags dependency_flags) {
    vkCmdPipelineBarrier(command_buffer, src_stage_flags, dst_stage_flags, dependency_flags, memory_barriers.size(), memory_barriers.data(),
                         buffer_barriers.size(), buffer_barriers.data(), image_barriers.size(), image_barriers.data());
}

VkResult timeline_semaphore_create(VkDevice device, uint64_t initial_timeline_value, VkSemaphore* semaphore, const void* pNext) {
    VkSemaphoreTypeCreateInfoKHR semaphore_type_create_info{};
    semaphore_type_create_info.sType         = VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO_KHR;
    semaphore_type_create_info.semaphoreType = VK_SEMAPHORE_TYPE_TIMELINE;
    semaphore_type_create_info.initialValue  = initial_timeline_value;
    semaphore_type_create_info.pNext         = pNext;

    return semaphore_create(device, semaphore, &semaphore_type_create_info);
}

VkImageMemoryBarrier2KHR image_memory_barrier_2_create(VkImage image, VkImageSubresourceRange subresource_range, VkImageLayout old_layout,
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
    image_mem_barrier_2.subresourceRange    = subresource_range;
    image_mem_barrier_2.pNext               = pNext;

    return image_mem_barrier_2;
}

VkBufferMemoryBarrier2KHR buffer_memory_barrier_2_create(VkBuffer buffer, uint32_t src_queue_family_index, uint32_t dst_queue_family_index,
                                                         VkPipelineStageFlags2 src_stage_flags, VkPipelineStageFlags2 dst_stage_flags,
                                                         VkAccessFlags2KHR src_access_flags, VkAccessFlags2KHR dst_access_flags, uint64_t offset,
                                                         uint64_t size, const void* pNext) {
    VkBufferMemoryBarrier2KHR buffer_memory_barrier_2{};
    buffer_memory_barrier_2.sType               = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER_2_KHR;
    buffer_memory_barrier_2.srcStageMask        = src_stage_flags;
    buffer_memory_barrier_2.srcAccessMask       = src_access_flags;
    buffer_memory_barrier_2.dstStageMask        = dst_stage_flags;
    buffer_memory_barrier_2.dstAccessMask       = dst_access_flags;
    buffer_memory_barrier_2.srcQueueFamilyIndex = src_queue_family_index;
    buffer_memory_barrier_2.dstQueueFamilyIndex = dst_queue_family_index;
    buffer_memory_barrier_2.buffer              = buffer;
    buffer_memory_barrier_2.offset              = offset;
    buffer_memory_barrier_2.size                = size;
    buffer_memory_barrier_2.pNext               = pNext;

    return buffer_memory_barrier_2;
}

VkMemoryBarrier2KHR global_memory_barrier_2_create(VkPipelineStageFlags2KHR src_stages, VkAccessFlags2KHR src_access,
                                                   VkPipelineStageFlags2KHR dst_stages, VkAccessFlags2KHR dst_access) {
    VkMemoryBarrier2KHR memory_barrier{};
    memory_barrier.sType         = VK_STRUCTURE_TYPE_MEMORY_BARRIER_2_KHR;
    memory_barrier.srcStageMask  = src_stages;
    memory_barrier.srcAccessMask = src_access;
    memory_barrier.dstStageMask  = dst_stages;
    memory_barrier.dstAccessMask = dst_access;

    return memory_barrier;
}

VkDependencyInfoKHR dependency_info_batch_create(std::span<VkImageMemoryBarrier2KHR>  image_barriers,
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

    return dependency_info;
}

VkDependencyInfoKHR dependency_info_create(const VkImageMemoryBarrier2KHR* image_barrier, const VkBufferMemoryBarrier2KHR* buffer_barrier,
                                           const VkMemoryBarrier2KHR* memory_barrier, VkDependencyFlags dependency_flags) {
    VkDependencyInfoKHR dependency_info{};
    dependency_info.sType                    = VK_STRUCTURE_TYPE_DEPENDENCY_INFO_KHR;
    dependency_info.pNext                    = nullptr;
    dependency_info.pImageMemoryBarriers     = image_barrier;
    dependency_info.imageMemoryBarrierCount  = image_barrier == nullptr ? 0 : 1;
    dependency_info.pBufferMemoryBarriers    = buffer_barrier;
    dependency_info.bufferMemoryBarrierCount = buffer_barrier == nullptr ? 0 : 1;
    dependency_info.pMemoryBarriers          = memory_barrier;
    dependency_info.memoryBarrierCount       = memory_barrier == nullptr ? 0 : 1;
    dependency_info.dependencyFlags          = dependency_flags;

    return dependency_info;
}

void pipeline_barrier_2_insert(VkCommandBuffer command_buffer, const VkDependencyInfoKHR* dependency_info) {
    vkCmdPipelineBarrier2(command_buffer, dependency_info);
}

} // namespace vk_lib