/*
 * Utilities regarding graphics presentation
 */

#pragma once
#include <vk_lib/common.h>

VkResult physical_device_get_surface_capabilities(VkPhysicalDevice physical_device, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* capabilities);

VkResult physical_device_get_surface_support(VkPhysicalDevice physical_device, VkSurfaceKHR surface, uint32_t queue_family_index,
                                             bool* present_supported);

VkResult physical_device_get_surface_formats(VkPhysicalDevice physical_device, VkSurfaceKHR surface, std::vector<VkSurfaceFormatKHR>* formats);

VkResult physical_device_get_present_modes(VkPhysicalDevice physical_device, VkSurfaceKHR surface, std::vector<VkPresentModeKHR>* present_modes);

struct SwapchainBuilder {
    VkSwapchainCreateInfoKHR swapchain_create_info{VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR};
    std::vector<uint32_t>    queue_family_indices{};
};

void swapchain_builder_set_surface_properties(SwapchainBuilder* builder, VkSurfaceKHR surface, VkSurfaceTransformFlagBitsKHR pre_transform,
                                              VkSwapchainCreateFlagsKHR create_flags = 0, bool clipped = true);

void swapchain_builder_set_image_properties(SwapchainBuilder* builder, VkFormat format, VkColorSpaceKHR color_space, uint32_t width, uint32_t height,
                                            uint32_t min_image_count, VkImageUsageFlags usage, uint32_t array_layers = 1);

void swapchain_builder_set_sharing_mode(SwapchainBuilder* builder, VkSharingMode sharing_mode, std::span<uint32_t> queue_family_indices = {});

void swapchain_builder_set_presentation_properties(SwapchainBuilder* builder, VkPresentModeKHR present_mode,
                                                   VkCompositeAlphaFlagBitsKHR composite_alpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
                                                   VkSwapchainKHR              old_swapchain   = nullptr);

void swapchain_builder_pNext(SwapchainBuilder* builder, const void* pNext);

void swapchain_builder_clear(SwapchainBuilder* builder);

VkResult swapchain_builder_swapchain_create(const SwapchainBuilder* builder, VkDevice device, VkSwapchainKHR* swapchain);

void swapchain_destroy(VkDevice device, VkSwapchainKHR swapchain);

VkResult swapchain_get_images(VkDevice device, VkSwapchainKHR swapchain, std::vector<VkImage>* images);

VkResult swapchain_acquire_next_image(VkDevice device, VkSwapchainKHR swapchain, uint32_t* image_index, VkSemaphore semaphore = nullptr,
                                      VkFence fence = nullptr, uint64_t timeout = UINT64_MAX);

VkPresentInfoKHR present_info_batch_create(std::span<VkSwapchainKHR> swapchains, std::span<uint32_t> image_indices,
                                           std::span<VkSemaphore> wait_semaphores = {}, std::vector<VkResult>* results = nullptr,
                                           const void* pNext = nullptr);

VkPresentInfoKHR present_info_create(const VkSwapchainKHR* swapchain, const uint32_t* image_index, const VkSemaphore* wait_semaphore = nullptr,
                                     const void* pNext = nullptr);

VkResult queue_present(VkQueue present_queue, const VkPresentInfoKHR* present_info);

void surface_destroy(VkInstance instance, VkSurfaceKHR surface);
