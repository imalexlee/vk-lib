/*
* Utilities regarding instance, device, and queue management
*/

#pragma once
#include <vk_lib/common.h>

// BEGIN INSTANCE

struct InstanceBuilder
{
    uint32_t api_version{};
    uint32_t app_version{};
    uint32_t engine_version{};
    std::string app_name{};
    std::string engine_name{};
    std::vector<std::string> instance_extensions{};
    std::vector<std::string> layers{};
    const void* instance_pNext;
    VkInstanceCreateFlags instance_create_flags;
};

void instance_builder_set_names(InstanceBuilder* builder, std::string_view app_name,
                                std::string_view engine_name);

void instance_builder_set_versions(InstanceBuilder* builder, uint32_t api, uint32_t app,
                                   uint32_t engine);

void instance_builder_set_instance_flags(InstanceBuilder* builder,
                                         VkInstanceCreateFlags instance_create_flags);

void instance_builder_set_instance_extensions(InstanceBuilder* builder,
                                              std::span<const char*> instance_extensions);

void instance_builder_set_layers(InstanceBuilder* builder,
                                 std::span<const char*> layers);

void instance_builder_set_instance_pNext(InstanceBuilder* builder, const void* pNext);

void instance_builder_clear(InstanceBuilder* builder);

VkResult instance_builder_instance_create(const InstanceBuilder* builder, VkInstance* instance);

void instance_destroy(VkInstance instance);

VkResult instance_enumerate_layer_properties(std::vector<VkLayerProperties>* layer_properties);


// END INSTANCE

// BEGIN PHYSICAL DEVICE MANAGEMENT


VkResult physical_device_enumerate_devices(VkInstance instance,
                                           std::vector<VkPhysicalDevice>* physical_devices);


VkPhysicalDeviceProperties physical_device_get_properties(VkPhysicalDevice physical_device);

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


void logical_device_builder_set_device_features_1(LogicalDeviceBuilder* builder,
                                                VkPhysicalDeviceFeatures features,
                                                void* extended_feature_chain);

void logical_device_builder_set_device_features_2(LogicalDeviceBuilder* builder,
                                                 VkPhysicalDeviceFeatures2 features_2);

void logical_device_builder_queue_create(LogicalDeviceBuilder* builder, uint32_t queue_family_index,
                                         float priority);

void logical_device_builder_set_device_extensions(LogicalDeviceBuilder* builder,
                                                  std::span<const char*> device_extensions);

VkResult logical_device_builder_device_create(LogicalDeviceBuilder* builder,
                                              VkPhysicalDevice physical_device, VkDevice* device);

void logical_device_destroy(VkDevice device);

// END LOGICAL DEVICE BUILDER

// BEGIN LOGICAL DEVICE MANAGEMENT

[[nodiscard]] VkQueue queue_get(VkDevice device, uint32_t queue_family_index,
                                uint32_t queue_index);


// END LOGICAL DEVICE MANAGEMENT
