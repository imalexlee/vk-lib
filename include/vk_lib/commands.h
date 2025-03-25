/*
 * Utilities command and command buffer management
 */

#pragma once
#include <vk_lib/common.h>

VkResult command_pool_create(VkDevice device, uint32_t queue_family_index, VkCommandPoolCreateFlags flags, VkCommandPool* command_pool);

VkResult command_pool_reset(VkDevice device, VkCommandPool command_pool, VkCommandPoolResetFlags flags = 0);

void command_pool_destroy(VkDevice device, VkCommandPool command_pool);

VkResult command_buffer_allocate(VkDevice device, VkCommandPool command_pool, VkCommandBufferLevel level, VkCommandBuffer* command_buffer);

VkResult command_buffer_begin(VkCommandBuffer command_buffer, VkCommandBufferUsageFlags flags,
                              const VkCommandBufferInheritanceInfo* inheritance_info = nullptr, const void* pNext = nullptr);

VkResult command_buffer_reset(VkCommandBuffer command_buffer, VkCommandBufferResetFlags flags = 0);

[[nodiscard]] VkCommandBufferSubmitInfoKHR command_buffer_submit_info_create_2(VkCommandBuffer command_buffer, uint32_t device_mask = 0,
                                                                               const void* pNext = nullptr);

struct SubmitInfoBuilder {
    VkSubmitInfo                      submit_info_1{VK_STRUCTURE_TYPE_SUBMIT_INFO};
    std::vector<VkSemaphore>          wait_semaphores_1{};
    std::vector<VkPipelineStageFlags> wait_dst_stage_masks_1{};
    std::vector<VkSemaphore>          signal_semaphores_1{};
    std::vector<VkCommandBuffer>      command_buffers_1{};

    VkSubmitInfo2KHR submit_info_2{VK_STRUCTURE_TYPE_SUBMIT_INFO_2_KHR};

    std::vector<VkSemaphoreSubmitInfoKHR>     wait_semaphores_2{};
    std::vector<VkSemaphoreSubmitInfoKHR>     signal_semaphores_2{};
    std::vector<VkCommandBufferSubmitInfoKHR> command_buffer_submit_infos_2{};
};

void submit_info_builder_batch_set_semaphores_1(SubmitInfoBuilder* builder, std::span<VkSemaphore> wait_semaphores,
                                                std::span<VkPipelineStageFlags> wait_dst_stage_masks, std::span<VkSemaphore> signal_semaphores);

void submit_info_builder_set_semaphores_1(SubmitInfoBuilder* builder, VkSemaphore wait_semaphore, VkPipelineStageFlags wait_dst_stage_mask,
                                          VkSemaphore signal_semaphore);

void submit_info_builder_batch_set_command_buffers_1(SubmitInfoBuilder* builder, std::span<VkCommandBuffer> command_buffers);

void submit_info_builder_set_command_buffer_1(SubmitInfoBuilder* builder, VkCommandBuffer command_buffer);

void submit_info_builder_set_pNext_1(SubmitInfoBuilder* builder, const void* pNext);

void submit_info_builder_batch_set_semaphore_infos_2(SubmitInfoBuilder* builder, std::span<VkSemaphoreSubmitInfoKHR> wait_semaphore_submit_infos,
                                                     std::span<VkSemaphoreSubmitInfoKHR> signal_semaphore_submit_infos);

void submit_info_builder_set_semaphore_infos_2(SubmitInfoBuilder* builder, const VkSemaphoreSubmitInfoKHR* wait_semaphore_submit_info,
                                               const VkSemaphoreSubmitInfoKHR* signal_semaphore_submit_info);

void submit_info_builder_batch_set_command_buffer_infos_2(SubmitInfoBuilder*                      builder,
                                                          std::span<VkCommandBufferSubmitInfoKHR> command_buffer_submit_infos);

void submit_info_builder_set_command_buffer_info_2(SubmitInfoBuilder* builder, const VkCommandBufferSubmitInfoKHR* command_buffer_submit_info);

void submit_info_builder_set_submit_flags(SubmitInfoBuilder* builder, VkSubmitFlagsKHR flags);

void submit_info_builder_set_pNext_2(SubmitInfoBuilder* builder, const void* pNext);

void submit_info_builder_clear(SubmitInfoBuilder* builder);

[[nodiscard]] VkSubmitInfo submit_info_builder_create_submit_info(const SubmitInfoBuilder* builder);

[[nodiscard]] VkSubmitInfo2KHR submit_info_builder_create_submit_info_2(const SubmitInfoBuilder* builder);

VkResult queue_batch_submit(VkQueue queue, std::span<VkSubmitInfo> submit_infos, VkFence fence = nullptr);

VkResult queue_submit(VkQueue queue, const VkSubmitInfo* submit_info, VkFence fence = nullptr);

VkResult queue_batch_submit_2(VkQueue queue, std::span<VkSubmitInfo2KHR> submit_infos, VkFence fence = nullptr);

VkResult queue_submit_2(VkQueue queue, const VkSubmitInfo2KHR* submit_info, VkFence fence = nullptr);
