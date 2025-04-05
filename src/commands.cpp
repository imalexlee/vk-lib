#include <vk_lib/commands.h>

namespace vk_lib {

VkCommandPoolCreateInfo command_pool_create_info(uint32_t queue_family_index, VkCommandPoolCreateFlags flags) {
    VkCommandPoolCreateInfo command_pool_create_info{};
    command_pool_create_info.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    command_pool_create_info.queueFamilyIndex = queue_family_index;
    command_pool_create_info.flags            = flags;

    return command_pool_create_info;
}

VkCommandBufferAllocateInfo command_buffer_allocate_info(VkCommandPool command_pool, VkCommandBufferLevel level, uint32_t command_buffer_count) {
    VkCommandBufferAllocateInfo command_buffer_allocate_info{};
    command_buffer_allocate_info.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    command_buffer_allocate_info.level              = level;
    command_buffer_allocate_info.commandBufferCount = command_buffer_count;
    command_buffer_allocate_info.commandPool        = command_pool;

    return command_buffer_allocate_info;
}

VkCommandBufferBeginInfo command_buffer_begin_info(VkCommandBufferUsageFlags flags, const VkCommandBufferInheritanceInfo* inheritance_info,
                                                   const void* pNext) {
    VkCommandBufferBeginInfo command_buffer_begin_info{};
    command_buffer_begin_info.sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    command_buffer_begin_info.pNext            = pNext;
    command_buffer_begin_info.flags            = flags;
    command_buffer_begin_info.pInheritanceInfo = inheritance_info;

    return command_buffer_begin_info;
}

VkCommandBufferSubmitInfoKHR command_buffer_submit_info(VkCommandBuffer command_buffer, uint32_t device_mask, const void* pNext) {
    VkCommandBufferSubmitInfoKHR command_buffer_submit_info{};
    command_buffer_submit_info.sType         = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO_KHR;
    command_buffer_submit_info.commandBuffer = command_buffer;
    command_buffer_submit_info.deviceMask    = device_mask;
    command_buffer_submit_info.pNext         = pNext;

    return command_buffer_submit_info;
}

VkSubmitInfo submit_info_batch(std::span<VkCommandBuffer> command_buffers, std::span<VkSemaphore> wait_semaphores,
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

VkSubmitInfo submit_info(const VkCommandBuffer* command_buffer, const VkSemaphore* wait_semaphore,
                         const VkPipelineStageFlags* wait_semaphore_stage_flags, const VkSemaphore* signal_semaphore, const void* pNext) {
    VkSubmitInfo submit_info{};
    submit_info.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.commandBufferCount   = 1;
    submit_info.pCommandBuffers      = command_buffer;
    submit_info.waitSemaphoreCount   = 1;
    submit_info.pWaitSemaphores      = wait_semaphore;
    submit_info.pWaitDstStageMask    = wait_semaphore_stage_flags;
    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores    = signal_semaphore;
    submit_info.pNext                = pNext;

    return submit_info;
}

VkSubmitInfo2KHR submit_info_2_batch(std::span<VkCommandBufferSubmitInfoKHR> command_buffer_submit_infos,
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

[[nodiscard]] VkSubmitInfo2KHR submit_info_2(const VkCommandBufferSubmitInfoKHR* command_buffer_submit_info,
                                             const VkSemaphoreSubmitInfoKHR*     wait_semaphores_submit_info,
                                             const VkSemaphoreSubmitInfoKHR* signal_semaphores_submit_info, VkSubmitFlagsKHR submit_flags,
                                             const void* pNext) {
    VkSubmitInfo2KHR submit_info_2{};
    submit_info_2.sType                    = VK_STRUCTURE_TYPE_SUBMIT_INFO_2_KHR;
    submit_info_2.commandBufferInfoCount   = 1;
    submit_info_2.pCommandBufferInfos      = command_buffer_submit_info;
    submit_info_2.waitSemaphoreInfoCount   = 1;
    submit_info_2.pWaitSemaphoreInfos      = wait_semaphores_submit_info;
    submit_info_2.signalSemaphoreInfoCount = 1;
    submit_info_2.pSignalSemaphoreInfos    = signal_semaphores_submit_info;
    submit_info_2.flags                    = submit_flags;
    submit_info_2.pNext                    = pNext;

    return submit_info_2;
}

} // namespace vk_lib