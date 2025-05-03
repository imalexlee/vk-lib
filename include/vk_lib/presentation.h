/*
 * Utilities regarding graphics presentation
 */

#pragma once
#include <vk_lib/common.h>

namespace vk_lib {

[[nodiscard]] VkPresentInfoKHR present_info_batch(std::span<const VkSwapchainKHR> swapchains, std::span<const uint32_t> image_indices,
                                                  std::span<const VkSemaphore> wait_semaphores = {}, std::vector<VkResult>* results = nullptr,
                                                  const void* pNext = nullptr);

[[nodiscard]] VkPresentInfoKHR present_info(const VkSwapchainKHR* swapchain, const uint32_t* image_index, const VkSemaphore* wait_semaphore = nullptr,
                                            const void* pNext = nullptr);

[[nodiscard]] VkSwapchainCreateInfoKHR
swapchain_create_info(VkSurfaceKHR surface, uint32_t min_image_count, VkFormat format, VkColorSpaceKHR color_space, VkExtent2D extent,
                      VkSurfaceTransformFlagBitsKHR pre_transform, VkPresentModeKHR present_mode = VK_PRESENT_MODE_FIFO_KHR,
                      VkImageUsageFlags usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, uint32_t array_layers = 1,
                      VkSharingMode sharing_mode = VK_SHARING_MODE_EXCLUSIVE, std::span<const uint32_t> queue_family_indices = {},
                      VkCompositeAlphaFlagBitsKHR composite_alpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR, bool clipped = false,
                      VkSwapchainKHR old_swapchain = nullptr);

} // namespace vk_lib