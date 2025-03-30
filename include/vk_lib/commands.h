/*
 * Utilities command and command buffer management
 */

#pragma once
#include <vk_lib/common.h>

VkResult command_pool_create(VkDevice device, uint32_t queue_family_index, VkCommandPoolCreateFlags flags, VkCommandPool* command_pool);

VkResult command_pool_reset(VkDevice device, VkCommandPool command_pool, VkCommandPoolResetFlags flags = 0);

void command_pool_destroy(VkDevice device, VkCommandPool command_pool);

VkResult command_buffer_allocate(VkDevice device, VkCommandPool command_pool, VkCommandBufferLevel level, VkCommandBuffer* command_buffer);

VkResult command_buffer_begin(VkCommandBuffer command_buffer, VkCommandBufferUsageFlags flags = 0,
                              const VkCommandBufferInheritanceInfo* inheritance_info = nullptr, const void* pNext = nullptr);

VkResult command_buffer_end(VkCommandBuffer command_buffer);

VkResult command_buffer_reset(VkCommandBuffer command_buffer, VkCommandBufferResetFlags flags = 0);

[[nodiscard]] VkSubmitInfo submit_info_create(std::span<VkCommandBuffer> command_buffers, std::span<VkSemaphore> wait_semaphores = {},
                                              std::span<VkPipelineStageFlags> wait_semaphore_stage_flags = {},
                                              std::span<VkSemaphore> signal_semaphores = {}, const void* pNext = nullptr);

VkResult queue_batch_submit(VkQueue queue, std::span<VkSubmitInfo> submit_infos, VkFence fence = nullptr);

VkResult queue_submit(VkQueue queue, const VkSubmitInfo* submit_info, VkFence fence = nullptr);

/*
 * CORE EXTENSIONS
 */

// VULKAN 1.3

[[nodiscard]] VkCommandBufferSubmitInfoKHR command_buffer_submit_info_2_create(VkCommandBuffer command_buffer, uint32_t device_mask = 0,
                                                                               const void* pNext = nullptr);

[[nodiscard]] VkSubmitInfo2KHR submit_info_2_create(std::span<VkCommandBufferSubmitInfoKHR> command_buffer_submit_infos,
                                                    std::span<VkSemaphoreSubmitInfoKHR>     wait_semaphores_submit_infos   = {},
                                                    std::span<VkSemaphoreSubmitInfoKHR>     signal_semaphores_submit_infos = {},
                                                    VkSubmitFlagsKHR submit_flags = 0, const void* pNext = nullptr);

VkResult queue_batch_submit_2(VkQueue queue, std::span<VkSubmitInfo2KHR> submit_infos, VkFence fence = nullptr);

VkResult queue_submit_2(VkQueue queue, const VkSubmitInfo2KHR* submit_info, VkFence fence = nullptr);
