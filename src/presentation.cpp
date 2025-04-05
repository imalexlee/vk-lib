
#include <vk_lib/presentation.h>

namespace vk_lib {

VkResult get_physical_device_surface_formats(VkPhysicalDevice physical_device, VkSurfaceKHR surface, std::vector<VkSurfaceFormatKHR>* formats) {
    uint32_t       format_count = 0;
    const VkResult result       = vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count, nullptr);
    if (result != VK_SUCCESS) {
        return result;
    }

    formats->resize(format_count);
    return vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count, formats->data());
}

VkResult get_physical_device_present_modes(VkPhysicalDevice physical_device, VkSurfaceKHR surface, std::vector<VkPresentModeKHR>* present_modes) {
    uint32_t       present_mode_count = 0;
    const VkResult result             = vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_mode_count, nullptr);
    if (result != VK_SUCCESS) {
        return result;
    }

    present_modes->resize(present_mode_count);
    return vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_mode_count, present_modes->data());
}

VkResult get_swapchain_images(VkDevice device, VkSwapchainKHR swapchain, std::vector<VkImage>* images) {
    uint32_t       swapchain_image_count = 0;
    const VkResult result                = vkGetSwapchainImagesKHR(device, swapchain, &swapchain_image_count, nullptr);
    if (result != VK_SUCCESS) {
        return result;
    }
    images->resize(swapchain_image_count);
    return vkGetSwapchainImagesKHR(device, swapchain, &swapchain_image_count, images->data());
}

VkPresentInfoKHR present_info_batch(std::span<VkSwapchainKHR> swapchains, std::span<uint32_t> image_indices, std::span<VkSemaphore> wait_semaphores,
                                    std::vector<VkResult>* results, const void* pNext) {
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

VkPresentInfoKHR present_info(const VkSwapchainKHR* swapchain, const uint32_t* image_index, const VkSemaphore* wait_semaphore, const void* pNext) {
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

VkSwapchainCreateInfoKHR swapchain_create_info(VkSurfaceKHR surface, uint32_t min_image_count, VkFormat format, VkColorSpaceKHR color_space,
                                               VkExtent2D extent, VkSurfaceTransformFlagBitsKHR pre_transform, VkPresentModeKHR present_mode,
                                               VkImageUsageFlags usage, uint32_t array_layers, VkSharingMode sharing_mode,
                                               std::span<uint32_t> queue_family_indices, VkCompositeAlphaFlagBitsKHR composite_alpha, bool clipped,
                                               VkSwapchainKHR old_swapchain) {
    VkSwapchainCreateInfoKHR swapchain_create_info{};
    swapchain_create_info.sType                 = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchain_create_info.surface               = surface;
    swapchain_create_info.minImageCount         = min_image_count;
    swapchain_create_info.imageFormat           = format;
    swapchain_create_info.imageColorSpace       = color_space;
    swapchain_create_info.imageExtent           = extent;
    swapchain_create_info.imageArrayLayers      = array_layers;
    swapchain_create_info.imageUsage            = usage;
    swapchain_create_info.imageSharingMode      = sharing_mode;
    swapchain_create_info.queueFamilyIndexCount = queue_family_indices.size();
    swapchain_create_info.pQueueFamilyIndices   = queue_family_indices.data();
    swapchain_create_info.preTransform          = pre_transform;
    swapchain_create_info.compositeAlpha        = composite_alpha;
    swapchain_create_info.presentMode           = present_mode;
    swapchain_create_info.clipped               = clipped;
    swapchain_create_info.oldSwapchain          = old_swapchain;

    return swapchain_create_info;
}

} // namespace vk_lib