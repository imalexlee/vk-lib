#include <vk_lib/synchronization.h>

namespace vk_lib {

VkSemaphoreCreateInfo semaphore_create_info(const void* pNext) {
    VkSemaphoreCreateInfo semaphore_create_info{};
    semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphore_create_info.flags = 0;
    semaphore_create_info.pNext = pNext;

    return semaphore_create_info;
}

VkFenceCreateInfo fence_create_info(VkFenceCreateFlags flags, const void* pNext) {
    VkFenceCreateInfo fence_create_info{};
    fence_create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_create_info.flags = flags;
    fence_create_info.pNext = pNext;

    return fence_create_info;
}

VkSemaphoreSubmitInfoKHR semaphore_submit_info(VkSemaphore semaphore, VkPipelineStageFlags2KHR stage_mask, uint64_t timeline_value,
                                               uint32_t device_index) {
    VkSemaphoreSubmitInfoKHR semaphore_submit_info{};
    semaphore_submit_info.sType       = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO_KHR;
    semaphore_submit_info.semaphore   = semaphore;
    semaphore_submit_info.stageMask   = stage_mask;
    semaphore_submit_info.deviceIndex = device_index;
    semaphore_submit_info.value       = timeline_value;
    semaphore_submit_info.pNext       = nullptr;

    return semaphore_submit_info;
}

VkImageMemoryBarrier image_memory_barrier(VkImage image, const VkImageSubresourceRange* subresource_range, VkImageLayout old_layout,
                                          VkImageLayout new_layout, uint32_t src_queue_family_index, uint32_t dst_queue_family_index,
                                          VkAccessFlags src_access_mask, VkAccessFlags dst_access_mask, const void* pNext) {
    VkImageMemoryBarrier image_mem_barrier{};
    image_mem_barrier.sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    image_mem_barrier.srcAccessMask       = src_access_mask;
    image_mem_barrier.dstAccessMask       = dst_access_mask;
    image_mem_barrier.image               = image;
    image_mem_barrier.oldLayout           = old_layout;
    image_mem_barrier.newLayout           = new_layout;
    image_mem_barrier.srcQueueFamilyIndex = src_queue_family_index;
    image_mem_barrier.dstQueueFamilyIndex = dst_queue_family_index;
    image_mem_barrier.subresourceRange    = *subresource_range;
    image_mem_barrier.pNext               = pNext;

    return image_mem_barrier;
}

VkBufferMemoryBarrier buffer_memory_barrier(VkBuffer buffer, uint32_t src_queue_family_index, uint32_t dst_queue_family_index,
                                            VkAccessFlags src_access_mask, VkAccessFlags dst_access_mask, uint64_t offset, uint64_t size,
                                            const void* pNext) {
    VkBufferMemoryBarrier buffer_mem_barrier{};
    buffer_mem_barrier.sType               = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
    buffer_mem_barrier.srcAccessMask       = src_access_mask;
    buffer_mem_barrier.dstAccessMask       = dst_access_mask;
    buffer_mem_barrier.srcQueueFamilyIndex = src_queue_family_index;
    buffer_mem_barrier.dstQueueFamilyIndex = dst_queue_family_index;
    buffer_mem_barrier.buffer              = buffer;
    buffer_mem_barrier.offset              = offset;
    buffer_mem_barrier.size                = size;
    buffer_mem_barrier.pNext               = pNext;

    return buffer_mem_barrier;
}

VkMemoryBarrier memory_barrier(VkAccessFlags src_access_mask, VkAccessFlags dst_access_mask) {
    VkMemoryBarrier mem_barrier{};
    mem_barrier.sType         = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
    mem_barrier.srcAccessMask = src_access_mask;
    mem_barrier.dstAccessMask = dst_access_mask;

    return mem_barrier;
}

VkEventCreateInfo event_create_info(VkEventCreateFlags flags, const void* pNext) {
    VkEventCreateInfo event_create_info{};
    event_create_info.sType = VK_STRUCTURE_TYPE_EVENT_CREATE_INFO;
    event_create_info.flags = flags;
    event_create_info.pNext = pNext;

    return event_create_info;
}

VkSemaphoreTypeCreateInfoKHR semaphore_type_create_info(VkSemaphoreType type, uint64_t initial_timeline_value, const void* pNext) {
    VkSemaphoreTypeCreateInfoKHR semaphore_type_create_info{};
    semaphore_type_create_info.sType         = VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO_KHR;
    semaphore_type_create_info.semaphoreType = type;
    semaphore_type_create_info.initialValue  = initial_timeline_value;
    semaphore_type_create_info.pNext         = pNext;

    return semaphore_type_create_info;
}

VkImageMemoryBarrier2KHR image_memory_barrier_2(VkImage image, VkImageSubresourceRange subresource_range, VkImageLayout old_layout,
                                                VkImageLayout new_layout, uint32_t src_queue_family_index, uint32_t dst_queue_family_index,
                                                VkPipelineStageFlags2 src_stage_mask, VkPipelineStageFlags2 dst_stage_mask,
                                                VkAccessFlags2 src_access_mask, VkAccessFlags2 dst_access_mask, const void* pNext) {
    VkImageMemoryBarrier2KHR image_mem_barrier_2{};
    image_mem_barrier_2.sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2_KHR;
    image_mem_barrier_2.srcStageMask        = src_stage_mask;
    image_mem_barrier_2.srcAccessMask       = src_access_mask;
    image_mem_barrier_2.dstStageMask        = dst_stage_mask;
    image_mem_barrier_2.dstAccessMask       = dst_access_mask;
    image_mem_barrier_2.image               = image;
    image_mem_barrier_2.oldLayout           = old_layout;
    image_mem_barrier_2.newLayout           = new_layout;
    image_mem_barrier_2.srcQueueFamilyIndex = src_queue_family_index;
    image_mem_barrier_2.dstQueueFamilyIndex = dst_queue_family_index;
    image_mem_barrier_2.subresourceRange    = subresource_range;
    image_mem_barrier_2.pNext               = pNext;

    return image_mem_barrier_2;
}

VkBufferMemoryBarrier2KHR buffer_memory_barrier_2(VkBuffer buffer, uint32_t src_queue_family_index, uint32_t dst_queue_family_index,
                                                  VkPipelineStageFlags2 src_stage_mask, VkPipelineStageFlags2 dst_stage_mask,
                                                  VkAccessFlags2KHR src_access_mask, VkAccessFlags2KHR dst_access_mask, uint64_t offset,
                                                  uint64_t size, const void* pNext) {
    VkBufferMemoryBarrier2KHR buffer_memory_barrier_2{};
    buffer_memory_barrier_2.sType               = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER_2_KHR;
    buffer_memory_barrier_2.srcStageMask        = src_stage_mask;
    buffer_memory_barrier_2.srcAccessMask       = src_access_mask;
    buffer_memory_barrier_2.dstStageMask        = dst_stage_mask;
    buffer_memory_barrier_2.dstAccessMask       = dst_access_mask;
    buffer_memory_barrier_2.srcQueueFamilyIndex = src_queue_family_index;
    buffer_memory_barrier_2.dstQueueFamilyIndex = dst_queue_family_index;
    buffer_memory_barrier_2.buffer              = buffer;
    buffer_memory_barrier_2.offset              = offset;
    buffer_memory_barrier_2.size                = size;
    buffer_memory_barrier_2.pNext               = pNext;

    return buffer_memory_barrier_2;
}

VkMemoryBarrier2KHR global_memory_barrier_2(VkPipelineStageFlags2KHR src_stage_mask, VkPipelineStageFlags2KHR dst_stage_mask,
                                            VkAccessFlags2KHR src_access_mask, VkAccessFlags2KHR dst_access_mask) {
    VkMemoryBarrier2KHR memory_barrier{};
    memory_barrier.sType         = VK_STRUCTURE_TYPE_MEMORY_BARRIER_2_KHR;
    memory_barrier.srcStageMask  = src_stage_mask;
    memory_barrier.srcAccessMask = src_access_mask;
    memory_barrier.dstStageMask  = dst_stage_mask;
    memory_barrier.dstAccessMask = dst_access_mask;

    return memory_barrier;
}

VkDependencyInfoKHR dependency_info_batch(std::span<const VkImageMemoryBarrier2KHR>  image_barriers,
                                          std::span<const VkBufferMemoryBarrier2KHR> buffer_barriers,
                                          std::span<const VkMemoryBarrier2KHR> memory_barriers, VkDependencyFlags dependency_flags) {
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

VkDependencyInfoKHR dependency_info(const VkImageMemoryBarrier2KHR* image_barrier, const VkBufferMemoryBarrier2KHR* buffer_barrier,
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

} // namespace vk_lib