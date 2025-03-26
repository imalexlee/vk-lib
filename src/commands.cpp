#include <vk_lib/commands.h>

VkResult command_pool_create(VkDevice device, uint32_t queue_family_index, VkCommandPoolCreateFlags flags, VkCommandPool* command_pool) {
    VkCommandPoolCreateInfo command_pool_create_info{};
    command_pool_create_info.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    command_pool_create_info.queueFamilyIndex = queue_family_index;
    command_pool_create_info.flags            = flags;

    return vkCreateCommandPool(device, &command_pool_create_info, nullptr, command_pool);
}

VkResult command_pool_reset(VkDevice device, VkCommandPool command_pool, VkCommandPoolResetFlags flags) {
    return vkResetCommandPool(device, command_pool, flags);
}

void command_pool_destroy(VkDevice device, VkCommandPool command_pool) { vkDestroyCommandPool(device, command_pool, nullptr); }

VkResult command_buffer_allocate(VkDevice device, VkCommandPool command_pool, VkCommandBufferLevel level, VkCommandBuffer* command_buffer) {
    VkCommandBufferAllocateInfo buffer_allocate_info{};
    buffer_allocate_info.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    buffer_allocate_info.level              = level;
    buffer_allocate_info.commandBufferCount = 1;
    buffer_allocate_info.commandPool        = command_pool;

    return vkAllocateCommandBuffers(device, &buffer_allocate_info, command_buffer);
}

VkResult command_buffer_begin(VkCommandBuffer command_buffer, VkCommandBufferUsageFlags flags, const VkCommandBufferInheritanceInfo* inheritance_info,
                              const void* pNext) {
    VkCommandBufferBeginInfo command_buffer_begin_info{};
    command_buffer_begin_info.sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    command_buffer_begin_info.pNext            = pNext;
    command_buffer_begin_info.flags            = flags;
    command_buffer_begin_info.pInheritanceInfo = inheritance_info;

    return vkBeginCommandBuffer(command_buffer, &command_buffer_begin_info);
}

VkResult command_buffer_reset(VkCommandBuffer command_buffer, VkCommandBufferResetFlags flags) { return vkResetCommandBuffer(command_buffer, flags); }

VkCommandBufferSubmitInfoKHR command_buffer_submit_info_2_create(VkCommandBuffer command_buffer, uint32_t device_mask, const void* pNext) {
    VkCommandBufferSubmitInfoKHR command_buffer_submit_info{};
    command_buffer_submit_info.sType         = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO_KHR;
    command_buffer_submit_info.commandBuffer = command_buffer;
    command_buffer_submit_info.deviceMask    = device_mask;
    command_buffer_submit_info.pNext         = pNext;

    return command_buffer_submit_info;
}

VkSubmitInfo submit_info_create(std::span<VkCommandBuffer> command_buffers, std::span<VkSemaphore> wait_semaphores,
                                std::span<VkPipelineStageFlags> wait_semaphore_stage_flags, std::span<VkSemaphore> signal_semaphores,
                                const void* pNext) {
    VkSubmitInfo submit_info{};
    submit_info.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.commandBufferCount   = command_buffers.size();
    submit_info.pCommandBuffers      = command_buffers.data();
    submit_info.waitSemaphoreCount   = wait_semaphores.size();
    submit_info.pWaitSemaphores      = wait_semaphores.data();
    submit_info.pWaitDstStageMask    = wait_semaphore_stage_flags.data();
    submit_info.signalSemaphoreCount = signal_semaphores.size();
    submit_info.pSignalSemaphores    = signal_semaphores.data();
    submit_info.pNext                = pNext;

    return submit_info;
}

VkSubmitInfo2KHR submit_info_2_create(std::span<VkCommandBufferSubmitInfoKHR> command_buffer_submit_infos,
                                      std::span<VkSemaphoreSubmitInfoKHR>     wait_semaphores_submit_infos,
                                      std::span<VkSemaphoreSubmitInfoKHR> signal_semaphores_submit_infos, VkSubmitFlagsKHR submit_flags,
                                      const void* pNext) {
    VkSubmitInfo2KHR submit_info_2{};
    submit_info_2.sType                    = VK_STRUCTURE_TYPE_SUBMIT_INFO_2_KHR;
    submit_info_2.commandBufferInfoCount   = command_buffer_submit_infos.size();
    submit_info_2.pCommandBufferInfos      = command_buffer_submit_infos.data();
    submit_info_2.waitSemaphoreInfoCount   = wait_semaphores_submit_infos.size();
    submit_info_2.pWaitSemaphoreInfos      = wait_semaphores_submit_infos.data();
    submit_info_2.signalSemaphoreInfoCount = signal_semaphores_submit_infos.size();
    submit_info_2.pSignalSemaphoreInfos    = signal_semaphores_submit_infos.data();
    submit_info_2.flags                    = submit_flags;
    submit_info_2.pNext                    = pNext;

    return submit_info_2;
}

VkResult queue_batch_submit(VkQueue queue, std::span<VkSubmitInfo> submit_infos, VkFence fence) {
    return vkQueueSubmit(queue, submit_infos.size(), submit_infos.data(), fence);
}

VkResult queue_submit(VkQueue queue, const VkSubmitInfo* submit_info, VkFence fence) { return vkQueueSubmit(queue, 1, submit_info, fence); }

VkResult queue_batch_submit_2(VkQueue queue, std::span<VkSubmitInfo2KHR> submit_infos, VkFence fence) {
    return vkQueueSubmit2(queue, submit_infos.size(), submit_infos.data(), fence);
}

VkResult queue_submit_2(VkQueue queue, const VkSubmitInfo2KHR* submit_info, VkFence fence) { return vkQueueSubmit2(queue, 1, submit_info, fence); }
