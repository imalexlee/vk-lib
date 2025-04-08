/*
 * Utilities regarding instance, device, and queue management
 */

#pragma once
#include <vk_lib/common.h>

namespace vk_lib {

[[nodiscard]] VkApplicationInfo application_info(const char* app_name, const char* engine_name, uint32_t api_version,
                                                 uint32_t app_version    = VK_MAKE_API_VERSION(0, 1, 0, 0),
                                                 uint32_t engine_version = VK_MAKE_API_VERSION(0, 1, 0, 0));

[[nodiscard]] VkInstanceCreateInfo instance_create_info(const VkApplicationInfo* app_info, std::span<const char*> layers = {},
                                                        std::span<const char*> instance_extensions = {}, VkInstanceCreateFlags flags = 0,
                                                        const void* pNext = nullptr);

VkResult create_instance_with_entrypoints(const VkInstanceCreateInfo* instance_create_info, VkInstance* instance);

VkResult enumerate_instance_layer_properties(std::vector<VkLayerProperties>* layer_properties);

VkResult enumerate_physical_devices(VkInstance instance, std::vector<VkPhysicalDevice>* physical_devices);

// index in vector represents the queue family index
[[nodiscard]] std::vector<VkQueueFamilyProperties> get_physical_device_queue_family_properties(VkPhysicalDevice physical_device);

[[nodiscard]] VkDeviceQueueCreateInfo device_queue_create_info(uint32_t family_index, uint32_t queue_count, std::span<const float> queue_priorities,
                                                               VkDeviceQueueCreateFlags flags = 0, const void* pNext = nullptr);

[[nodiscard]] VkDeviceCreateInfo device_create_info(std::span<const VkDeviceQueueCreateInfo> queue_create_infos,
                                                    std::span<const char*> device_extensions = {}, const VkPhysicalDeviceFeatures* features = nullptr,
                                                    const void* pNext = nullptr);

VkResult create_device_with_entrypoints(VkPhysicalDevice physical_device, const VkDeviceCreateInfo* device_create_info, VkDevice* device);

VkResult enumerate_device_extension_properties(VkPhysicalDevice physical_device, std::vector<VkExtensionProperties> extension_properties,
                                               const char* layer_name = nullptr);

} // namespace vk_lib