/*
* Utilities regarding instance, device, and queue management
*/

#pragma once
#include <vk_lib/common.h>

#define VK_CHECK(x)                                                                                                                                  \
    do {                                                                                                                                             \
        VkResult err = x;                                                                                                                            \
        if (err) {                                                                                                                                   \
            printf("Detected Vulkan error: %s", string_VkResult(err));                                                                               \
            abort();                                                                                                                                 \
        }                                                                                                                                            \
    } while (0)

// BEGIN INSTANCE

struct InstanceBuilder
{
    std::string app_name{};
    std::string engine_name{};
    uint32_t api_version{};
    uint32_t app_version{};
    uint32_t engine_version{};
    VkInstanceCreateFlags instance_create_flags{};
    std::vector<std::string> instance_extensions{};
    // DEBUGGING
    std::vector<std::string> validation_layers{};
    // debug messenger ext
    VkDebugUtilsMessengerCreateInfoEXT messenger_create_info{};
    // layer settings ext
    VkLayerSettingsCreateInfoEXT layer_settings_create_info{};
    std::vector<VkLayerSettingEXT> layer_settings{};
    // validation features ext
    VkValidationFeaturesEXT validation_features{};
    std::vector<VkValidationFeatureEnableEXT> enabled_validation_features{};
    std::vector<VkValidationFeatureDisableEXT> disabled_validation_features{};
    // validation flags ext
    std::vector<VkValidationCheckEXT> disabled_validation_checks{};
    VkValidationFlagsEXT validation_flags{};
};

void instance_builder_set_names(InstanceBuilder* builder, std::string_view app_name,
                                std::string_view engine_name);

void instance_builder_set_versions(InstanceBuilder* builder, uint32_t api, uint32_t app,
                                   uint32_t engine);

void instance_builder_set_instance_flags(InstanceBuilder* builder,
                                         VkInstanceCreateFlags instance_create_flags);

void instance_builder_set_instance_extensions(InstanceBuilder* builder,
                                              std::span<const char*> instance_extensions);

void instance_builder_set_validation_layers(InstanceBuilder* builder,
                                            std::span<const char*> validation_layers);

void instance_builder_set_debug_messenger(InstanceBuilder* builder,
                                          VkDebugUtilsMessageSeverityFlagsEXT severity,
                                          VkDebugUtilsMessageTypeFlagsEXT type,
                                          PFN_vkDebugUtilsMessengerCallbackEXT callback,
                                          void* user_data);

void instance_builder_set_layer_settings(InstanceBuilder* builder,
                                         std::span<VkLayerSettingEXT> layer_settings);

void instance_builder_set_validation_features(InstanceBuilder* builder,
                                              std::span<VkValidationFeatureEnableEXT>
                                              enabled_features,
                                              std::span<VkValidationFeatureDisableEXT>
                                              disabled_features);

void instance_builder_set_validation_flags(InstanceBuilder* builder,
                                           std::span<VkValidationCheckEXT>
                                           disabled_validation_checks);

VkResult instance_builder_instance_create(InstanceBuilder* builder, VkInstance* instance);


VkResult instance_enumerate_layer_properties(std::vector<VkLayerProperties>* layer_properties);

void instance_destroy(VkInstance instance);

// END INSTANCE

// BEGIN PHYSICAL DEVICE MANAGEMENT

struct PhysicalDevice
{
    VkPhysicalDevice physical_device{};
    VkPhysicalDeviceProperties physical_device_properties{};
};

VkResult physical_device_enumerate_devices(VkInstance instance,
                                           std::vector<PhysicalDevice>* physical_devices);

[[nodiscard]] VkPhysicalDeviceFeatures physical_device_get_features(
    VkPhysicalDevice physical_device);

[[nodiscard]] VkPhysicalDeviceFeatures2 physical_device_get_features2(
    VkPhysicalDevice physical_device, void* extended_feature_chain);

// index in vector represents the queue family index
[[nodiscard]] std::vector<VkQueueFamilyProperties> physical_device_enumerate_queue_families(
    VkPhysicalDevice physical_device);

// END PHYSICAL DEVICE MANAGEMENT

// BEGIN LOGICAL DEVICE BUILDER

struct LogicalDeviceBuilder
{
    // (family index, # queues to create from family)
    std::map<uint32_t, uint32_t> queue_family_creation_map;
    // (family index, priorities). lines up with the map above. Just here to preserve values
    std::map<uint32_t, std::vector<float>> queue_priorities_map;
    std::vector<std::string> device_extensions;
    std::optional<VkPhysicalDeviceFeatures> physical_device_features;
    std::optional<VkPhysicalDeviceFeatures2> physical_device_features2;
    void* extended_feature_chain = nullptr;
};

VkResult logical_device_builder_device_create(LogicalDeviceBuilder* builder,
                                              VkPhysicalDevice physical_device, VkDevice* device);

void logical_device_builder_set_device_features(LogicalDeviceBuilder* builder,
                                                VkPhysicalDeviceFeatures features,
                                                void* extended_feature_chain);

void logical_device_builder_set_device_features2(LogicalDeviceBuilder* builder,
                                                 VkPhysicalDeviceFeatures2 features2);

void logical_device_builder_queue_create(LogicalDeviceBuilder* builder, uint32_t queue_family_index,
                                         float priority);

void logical_device_builder_set_device_extensions(LogicalDeviceBuilder* builder,
                                                  std::span<const char*> device_extensions);

// END LOGICAL DEVICE BUILDER

// BEGIN LOGICAL DEVICE MANAGEMENT

[[nodiscard]] VkQueue logical_device_get_queue(VkDevice device, uint32_t queue_family_index,
                                               uint32_t queue_index);

// END LOGICAL DEVICE MANAGEMENT
