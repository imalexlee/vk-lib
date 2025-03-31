
#include <vk_lib/presentation.h>

VkResult physical_device_get_surface_capabilities(VkPhysicalDevice physical_device, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* capabilities) {
    return vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, capabilities);
}

VkResult physical_device_get_surface_support(VkPhysicalDevice physical_device, VkSurfaceKHR surface, uint32_t queue_family_index,
                                             bool* present_supported) {
    VkBool32       supported = VK_FALSE;
    const VkResult result    = vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, queue_family_index, surface, &supported);
    if (result != VK_SUCCESS) {
        return result;
    }
    *present_supported = supported;

    return VK_SUCCESS;
}

VkResult physical_device_get_surface_formats(VkPhysicalDevice physical_device, VkSurfaceKHR surface, std::vector<VkSurfaceFormatKHR>* formats) {
    uint32_t       format_count = 0;
    const VkResult result       = vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count, nullptr);
    if (result != VK_SUCCESS) {
        return result;
    }

    formats->clear();
    formats->resize(format_count);
    return vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count, formats->data());
}

VkResult physical_device_get_present_modes(VkPhysicalDevice physical_device, VkSurfaceKHR surface, std::vector<VkPresentModeKHR>* present_modes) {
    uint32_t       present_mode_count = 0;
    const VkResult result             = vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_mode_count, nullptr);
    if (result != VK_SUCCESS) {
        return result;
    }

    present_modes->clear();
    present_modes->resize(present_mode_count);
    return vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_mode_count, present_modes->data());
}

void swapchain_builder_set_surface_properties(SwapchainBuilder* builder, VkSurfaceKHR surface, VkSurfaceTransformFlagBitsKHR pre_transform,
                                              VkSwapchainCreateFlagsKHR create_flags, bool clipped) {
    builder->swapchain_create_info.surface      = surface;
    builder->swapchain_create_info.preTransform = pre_transform;
    builder->swapchain_create_info.flags        = create_flags;
    builder->swapchain_create_info.clipped      = clipped;
}

void swapchain_builder_set_image_properties(SwapchainBuilder* builder, VkFormat format, VkColorSpaceKHR color_space, uint32_t width, uint32_t height,
                                            uint32_t min_image_count, VkImageUsageFlags usage, uint32_t array_layers) {
    builder->swapchain_create_info.imageFormat      = format;
    builder->swapchain_create_info.imageColorSpace  = color_space;
    builder->swapchain_create_info.imageExtent      = {width, height};
    builder->swapchain_create_info.minImageCount    = min_image_count;
    builder->swapchain_create_info.imageUsage       = usage;
    builder->swapchain_create_info.imageArrayLayers = array_layers;
}

void swapchain_builder_set_sharing_mode(SwapchainBuilder* builder, VkSharingMode sharing_mode, std::span<uint32_t> queue_family_indices) {
    builder->swapchain_create_info.imageSharingMode = sharing_mode;

    if (!queue_family_indices.empty()) {
        builder->queue_family_indices.assign(queue_family_indices.begin(), queue_family_indices.end());
        builder->swapchain_create_info.queueFamilyIndexCount = builder->queue_family_indices.size();
        builder->swapchain_create_info.pQueueFamilyIndices   = builder->queue_family_indices.data();
    } else {
        builder->swapchain_create_info.queueFamilyIndexCount = 0;
        builder->swapchain_create_info.pQueueFamilyIndices   = nullptr;
    }
}

void swapchain_builder_set_presentation_properties(SwapchainBuilder* builder, VkPresentModeKHR present_mode,
                                                   VkCompositeAlphaFlagBitsKHR composite_alpha, VkSwapchainKHR old_swapchain) {
    builder->swapchain_create_info.presentMode    = present_mode;
    builder->swapchain_create_info.compositeAlpha = composite_alpha;
    builder->swapchain_create_info.oldSwapchain   = old_swapchain;
}

void swapchain_builder_pNext(SwapchainBuilder* builder, const void* pNext) { builder->swapchain_create_info.pNext = pNext; }

void swapchain_builder_clear(SwapchainBuilder* builder) { *builder = SwapchainBuilder{}; }

VkResult swapchain_builder_swapchain_create(const SwapchainBuilder* builder, VkDevice device, VkSwapchainKHR* swapchain) {
    return vkCreateSwapchainKHR(device, &builder->swapchain_create_info, nullptr, swapchain);
}

void swapchain_destroy(VkDevice device, VkSwapchainKHR swapchain) { vkDestroySwapchainKHR(device, swapchain, nullptr); }

VkResult swapchain_get_images(VkDevice device, VkSwapchainKHR swapchain, std::vector<VkImage>* images) {
    uint32_t       swapchain_image_count = 0;
    const VkResult result                = vkGetSwapchainImagesKHR(device, swapchain, &swapchain_image_count, nullptr);
    if (result != VK_SUCCESS) {
        return result;
    }
    images->resize(swapchain_image_count);
    return vkGetSwapchainImagesKHR(device, swapchain, &swapchain_image_count, images->data());
}

VkResult swapchain_acquire_next_image(VkDevice device, VkSwapchainKHR swapchain, uint32_t* image_index, VkSemaphore semaphore, VkFence fence,
                                      uint64_t timeout) {
    return vkAcquireNextImageKHR(device, swapchain, timeout, semaphore, fence, image_index);
}

VkPresentInfoKHR present_info_batch_create(std::span<VkSwapchainKHR> swapchains, std::span<uint32_t> image_indices,
                                           std::span<VkSemaphore> wait_semaphores, std::vector<VkResult>* results, const void* pNext) {
    VkPresentInfoKHR present_info{};
    present_info.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.swapchainCount     = swapchains.size();
    present_info.pSwapchains        = swapchains.data();
    present_info.waitSemaphoreCount = wait_semaphores.size();
    present_info.pWaitSemaphores    = wait_semaphores.data();
    present_info.pImageIndices      = image_indices.data();
    present_info.pNext              = pNext;
    if (results != nullptr) {
        results->resize(swapchains.size());
        present_info.pResults = results->data();
    }

    return present_info;
}

VkPresentInfoKHR present_info_create(const VkSwapchainKHR* swapchain, const uint32_t* image_index, const VkSemaphore* wait_semaphore,
                                     const void* pNext) {
    VkPresentInfoKHR present_info{};
    present_info.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.swapchainCount     = 1;
    present_info.pSwapchains        = swapchain;
    present_info.waitSemaphoreCount = wait_semaphore == nullptr ? 0 : 1;
    present_info.pWaitSemaphores    = wait_semaphore;
    present_info.pImageIndices      = image_index;
    present_info.pNext              = pNext;

    return present_info;
}

VkResult queue_present(VkQueue present_queue, const VkPresentInfoKHR* present_info) { return vkQueuePresentKHR(present_queue, present_info); }

void surface_destroy(VkInstance instance, VkSurfaceKHR surface) { vkDestroySurfaceKHR(instance, surface, nullptr); }