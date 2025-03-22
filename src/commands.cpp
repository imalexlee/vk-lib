#include <vk_lib/commands.h>

VkResult command_pool_create(VkDevice device, uint32_t queue_family_index, VkCommandPoolCreateFlags flags, VkCommandPool* command_pool) {
    VkCommandPoolCreateInfo command_pool_create_info{};
    command_pool_create_info.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    command_pool_create_info.queueFamilyIndex = queue_family_index;
    command_pool_create_info.flags            = flags;

    return vkCreateCommandPool(device, &command_pool_create_info, nullptr, command_pool);
}

void command_pool_destroy(VkDevice device, VkCommandPool command_pool) {
    vkDestroyCommandPool(device, command_pool, nullptr);
}

VkResult command_buffer_allocate(VkDevice device, VkCommandPool command_pool, VkCommandBufferLevel level, VkCommandBuffer* command_buffer) {
    VkCommandBufferAllocateInfo buffer_allocate_info{};
    buffer_allocate_info.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    buffer_allocate_info.level              = level;
    buffer_allocate_info.commandBufferCount = 1;
    buffer_allocate_info.commandPool        = command_pool;

    return vkAllocateCommandBuffers(device, &buffer_allocate_info, command_buffer);
}

VkResult command_buffer_begin(VkCommandBuffer command_buffer, VkCommandBufferUsageFlags flags, const VkCommandBufferInheritanceInfo* inheritance_info, const void* pNext) {
    VkCommandBufferBeginInfo command_buffer_begin_info{};
    command_buffer_begin_info.sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    command_buffer_begin_info.pNext            = pNext;
    command_buffer_begin_info.flags            = flags;
    command_buffer_begin_info.pInheritanceInfo = inheritance_info;

    return vkBeginCommandBuffer(command_buffer, &command_buffer_begin_info);
}

VkResult command_buffer_reset(VkCommandBuffer command_buffer, VkCommandBufferResetFlags flags ) {
    return vkResetCommandBuffer(command_buffer, flags);
}

VkCommandBufferSubmitInfoKHR command_buffer_submit_info_create(VkCommandBuffer command_buffer, uint32_t device_mask, const void* pNext) {
    VkCommandBufferSubmitInfoKHR command_buffer_submit_info{};
    command_buffer_submit_info.sType         = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO_KHR;
    command_buffer_submit_info.commandBuffer = command_buffer;
    command_buffer_submit_info.deviceMask    = device_mask;
    command_buffer_submit_info.pNext         = pNext;

    return command_buffer_submit_info;
}

void submit_info_builder_batch_set_semaphores_1(SubmitInfoBuilder* builder, std::span<VkSemaphore> wait_semaphores,
                                                std::span<VkPipelineStageFlags> wait_dst_stage_masks,
                                                std::span<VkSemaphore> signal_semaphores) {
    builder->wait_semaphores_1.assign(wait_semaphores.begin(), wait_semaphores.end());
    builder->wait_dst_stage_masks_1.assign(wait_dst_stage_masks.begin(), wait_dst_stage_masks.end());
    builder->signal_semaphores_1.assign(signal_semaphores.begin(), signal_semaphores.end());
}

void submit_info_builder_set_semaphores_1(SubmitInfoBuilder* builder, VkSemaphore wait_semaphore,
                                          VkPipelineStageFlags wait_dst_stage_mask,
                                          VkSemaphore signal_semaphore) {
    builder->wait_semaphores_1.resize(1);
    builder->wait_dst_stage_masks_1.resize(1);
    builder->signal_semaphores_1.resize(1);

    builder->wait_semaphores_1[0]      = wait_semaphore;
    builder->wait_dst_stage_masks_1[0] = wait_dst_stage_mask;
    builder->signal_semaphores_1[0]    = signal_semaphore;

}

void submit_info_builder_batch_set_command_buffers_1(SubmitInfoBuilder* builder, std::span<VkCommandBuffer> command_buffers) {
    builder->command_buffers_1.assign(command_buffers.begin(), command_buffers.end());
}

void submit_info_builder_set_command_buffer_1(SubmitInfoBuilder* builder, VkCommandBuffer command_buffer) {
    builder->command_buffers_1.resize(1);
    builder->command_buffers_1[0] = command_buffer;
}

void submit_info_builder_set_pNext_1(SubmitInfoBuilder* builder, const void* pNext) {
    builder->submit_info_1.pNext = pNext;
}

void submit_info_builder_batch_set_semaphore_infos_2(SubmitInfoBuilder* builder, std::span<VkSemaphoreSubmitInfoKHR> wait_semaphore_submit_infos,
                                                     std::span<VkSemaphoreSubmitInfoKHR> signal_semaphore_submit_infos) {
    builder->wait_semaphores_2.assign(wait_semaphore_submit_infos.begin(), wait_semaphore_submit_infos.end());
    builder->signal_semaphores_2.assign(signal_semaphore_submit_infos.begin(), signal_semaphore_submit_infos.end());
}

void submit_info_builder_set_semaphore_infos_2(SubmitInfoBuilder* builder, const VkSemaphoreSubmitInfoKHR* wait_semaphore_submit_info, const VkSemaphoreSubmitInfoKHR* signal_semaphore_submit_info) {
    builder->wait_semaphores_2.resize(1);
    builder->signal_semaphores_2.resize(1);

    builder->wait_semaphores_2[0]   = *wait_semaphore_submit_info;
    builder->signal_semaphores_2[0] = *signal_semaphore_submit_info;
}

void submit_info_builder_batch_set_command_buffer_infos_2(SubmitInfoBuilder* builder,
                                                          std::span<VkCommandBufferSubmitInfoKHR>
                                                          command_buffer_submit_infos) {
    builder->command_buffer_submit_infos_2.assign(command_buffer_submit_infos.begin(), command_buffer_submit_infos.end());
}

void submit_info_builder_set_command_buffer_info_2(SubmitInfoBuilder* builder,
                                                   const VkCommandBufferSubmitInfoKHR*
                                                   command_buffer_submit_info) {
    builder->command_buffer_submit_infos_2.resize(1);

    builder->command_buffer_submit_infos_2[0] = *command_buffer_submit_info;
}

void submit_info_builder_set_submit_flags(SubmitInfoBuilder* builder, VkSubmitFlagsKHR flags) {
    builder->submit_info_2.flags = flags;
}

void submit_info_builder_set_pNext_2(SubmitInfoBuilder* builder, const void* pNext) {
    builder->submit_info_2.pNext = pNext;
}

void submit_info_builder_clear(SubmitInfoBuilder* builder) {
    *builder = SubmitInfoBuilder{};
}

VkSubmitInfo submit_info_builder_create_submit_info(const SubmitInfoBuilder* builder) {
    VkSubmitInfo submit_info         = builder->submit_info_1;
    submit_info.pNext                = builder->submit_info_1.pNext;
    submit_info.waitSemaphoreCount   = builder->wait_semaphores_1.size();
    submit_info.pWaitSemaphores      = builder->wait_semaphores_1.data();
    submit_info.commandBufferCount   = builder->command_buffers_1.size();
    submit_info.pCommandBuffers      = builder->command_buffers_1.data();
    submit_info.signalSemaphoreCount = builder->signal_semaphores_1.size();
    submit_info.pSignalSemaphores    = builder->signal_semaphores_1.data();

    return submit_info;
}

VkSubmitInfo2KHR submit_info_builder_create_submit_info_2(const SubmitInfoBuilder* builder) {
    VkSubmitInfo2KHR submit_info_2         = builder->submit_info_2;
    submit_info_2.pNext                    = builder->submit_info_2.pNext;
    submit_info_2.flags                    = builder->submit_info_2.flags;
    submit_info_2.waitSemaphoreInfoCount   = builder->wait_semaphores_2.size();
    submit_info_2.pWaitSemaphoreInfos      = builder->wait_semaphores_2.data();
    submit_info_2.commandBufferInfoCount   = builder->command_buffer_submit_infos_2.size();
    submit_info_2.pCommandBufferInfos      = builder->command_buffer_submit_infos_2.data();
    submit_info_2.signalSemaphoreInfoCount = builder->signal_semaphores_2.size();
    submit_info_2.pSignalSemaphoreInfos    = builder->signal_semaphores_2.data();

    return submit_info_2;
}

VkResult queue_batch_submit(VkQueue queue, std::span<VkSubmitInfo> submit_infos, VkFence fence) {
    return vkQueueSubmit(queue, submit_infos.size(), submit_infos.data(), fence);
}

VkResult queue_submit(VkQueue queue, const VkSubmitInfo* submit_info, VkFence fence) {
    return vkQueueSubmit(queue, 1, submit_info, fence);
}

VkResult queue_batch_submit_2(VkQueue queue, std::span<VkSubmitInfo2KHR> submit_infos,
                              VkFence fence) {
    return vkQueueSubmit2(queue, submit_infos.size(), submit_infos.data(), fence);

}

VkResult queue_submit_2(VkQueue queue, const VkSubmitInfo2KHR* submit_info,
                        VkFence fence) {
    return vkQueueSubmit2(queue, 1, submit_info, fence);
}
