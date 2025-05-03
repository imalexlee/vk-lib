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

[[nodiscard]] VkDeviceQueueCreateInfo device_queue_create_info(uint32_t family_index, std::span<const float> queue_priorities,
                                                               VkDeviceQueueCreateFlags flags = 0, const void* pNext = nullptr);

[[nodiscard]] VkDeviceCreateInfo device_create_info(std::span<const VkDeviceQueueCreateInfo> queue_create_infos,
                                                    std::span<const char*> device_extensions = {}, const VkPhysicalDeviceFeatures* features = nullptr,
                                                    const void* pNext = nullptr);

} // namespace vk_lib