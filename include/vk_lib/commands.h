/*
 * Utilities command and command buffer management
 */

#pragma once
#include <vk_lib/common.h>

namespace vk_lib {

[[nodiscard]] VkCommandPoolCreateInfo command_pool_create_info(uint32_t queue_family_index, VkCommandPoolCreateFlags flags = 0);

[[nodiscard]] VkCommandBufferAllocateInfo command_buffer_allocate_info(VkCommandPool        command_pool,
                                                                       VkCommandBufferLevel level                = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
                                                                       uint32_t             command_buffer_count = 1);

[[nodiscard]] VkCommandBufferBeginInfo command_buffer_begin_info(VkCommandBufferUsageFlags             flags            = 0,
                                                                 const VkCommandBufferInheritanceInfo* inheritance_info = nullptr,
                                                                 const void*                           pNext            = nullptr);

[[nodiscard]] VkSubmitInfo submit_info_batch(std::span<const VkCommandBuffer> command_buffers, std::span<const VkSemaphore> wait_semaphores = {},
                                             std::span<const VkPipelineStageFlags> wait_semaphore_stage_flags = {},
                                             std::span<const VkSemaphore> signal_semaphores = {}, const void* pNext = nullptr);

[[nodiscard]] VkSubmitInfo submit_info(const VkCommandBuffer* command_buffer, const VkSemaphore* wait_semaphore = nullptr,
                                       const VkPipelineStageFlags* wait_semaphore_stage_flags = nullptr,
                                       const VkSemaphore* signal_semaphore = nullptr, const void* pNext = nullptr);
/*
 * CORE EXTENSIONS
 */

// VULKAN 1.3

[[nodiscard]] VkCommandBufferSubmitInfoKHR command_buffer_submit_info(VkCommandBuffer command_buffer, uint32_t device_mask = 0,
                                                                      const void* pNext = nullptr);

[[nodiscard]] VkSubmitInfo2KHR submit_info_2_batch(std::span<const VkCommandBufferSubmitInfoKHR> command_buffer_submit_infos,
                                                   std::span<const VkSemaphoreSubmitInfoKHR>     wait_semaphores_submit_infos   = {},
                                                   std::span<const VkSemaphoreSubmitInfoKHR>     signal_semaphores_submit_infos = {},
                                                   VkSubmitFlagsKHR submit_flags = 0, const void* pNext = nullptr);

[[nodiscard]] VkSubmitInfo2KHR submit_info_2(const VkCommandBufferSubmitInfoKHR* command_buffer_submit_info,
                                             const VkSemaphoreSubmitInfoKHR*     wait_semaphores_submit_info   = nullptr,
                                             const VkSemaphoreSubmitInfoKHR*     signal_semaphores_submit_info = nullptr,
                                             VkSubmitFlagsKHR submit_flags = 0, const void* pNext = nullptr);
} // namespace vk_lib