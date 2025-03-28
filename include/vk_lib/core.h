/*
 * Utilities regarding instance, device, and queue management
 */

#pragma once
#include <vk_lib/common.h>

struct InstanceBuilder {
    uint32_t                 api_version{};
    uint32_t                 app_version{};
    uint32_t                 engine_version{};
    std::string              app_name{};
    std::string              engine_name{};
    std::vector<std::string> instance_extensions{};
    std::vector<std::string> layers{};
    VkInstanceCreateFlags    instance_create_flags{};
    const void*              instance_pNext{};
};

void instance_builder_set_names(InstanceBuilder* builder, std::string_view app_name, std::string_view engine_name);

void instance_builder_set_versions(InstanceBuilder* builder, uint32_t api, uint32_t app = VK_MAKE_API_VERSION(0, 1, 0, 0),
                                   uint32_t engine = VK_MAKE_API_VERSION(0, 1, 0, 0));

void instance_builder_set_instance_flags(InstanceBuilder* builder, VkInstanceCreateFlags instance_create_flags);

void instance_builder_set_extensions(InstanceBuilder* builder, std::span<const char*> instance_extensions);

void instance_builder_set_layers(InstanceBuilder* builder, std::span<const char*> layers);

void instance_builder_set_instance_pNext(InstanceBuilder* builder, const void* pNext);

void instance_builder_clear(InstanceBuilder* builder);

VkResult instance_builder_instance_create(const InstanceBuilder* builder, VkInstance* instance);

void instance_destroy(VkInstance instance);

VkResult instance_enumerate_layer_properties(std::vector<VkLayerProperties>* layer_properties);

VkResult physical_device_enumerate_devices(VkInstance instance, std::vector<VkPhysicalDevice>* physical_devices);

[[nodiscard]] VkPhysicalDeviceProperties physical_device_get_properties(VkPhysicalDevice physical_device);

[[nodiscard]] VkPhysicalDeviceFeatures physical_device_get_features(VkPhysicalDevice physical_device);

// index in vector represents the queue family index
[[nodiscard]] std::vector<VkQueueFamilyProperties> physical_device_enumerate_queue_families(VkPhysicalDevice physical_device);

[[nodiscard]] VkDeviceQueueCreateInfo queue_create_info_create(uint32_t family_index, uint32_t queue_count, std::span<float> queue_priorities,
                                                               VkDeviceQueueCreateFlags flags = 0, const void* pNext = nullptr);

[[nodiscard]] VkQueue queue_get(VkDevice device, uint32_t queue_family_index, uint32_t queue_index);

void device_destroy(VkDevice device);

VkResult device_create(VkPhysicalDevice physical_device, std::span<VkDeviceQueueCreateInfo> queue_create_infos, VkDevice* device,
                       std::span<const char*> extension_names = {}, const VkPhysicalDeviceFeatures* enabled_features = nullptr,
                       const void* pNext = nullptr);

VkResult device_enumerate_extension_properties(VkPhysicalDevice physical_device, std::vector<VkExtensionProperties> extension_properties,
                                               std::string_view layer_name = {});

/*
 * CORE EXTENSIONS
 */

// VULKAN 1.1

[[nodiscard]] VkPhysicalDeviceFeatures2KHR physical_device_get_features_2(VkPhysicalDevice physical_device, void* extended_feature_chain);
