/*
* Utilities regarding instance, device, and queue management
*/

#pragma once
#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>
#include <vector>
#include <span>
#include <iostream>

#define VK_CHECK(x)                                                                                                                                  \
    do {                                                                                                                                             \
        VkResult err = x;                                                                                                                            \
        if (err) {                                                                                                                                   \
            printf("Detected Vulkan error: %s", string_VkResult(err));                                                                               \
            abort();                                                                                                                                 \
        }                                                                                                                                            \
    } while (0)


struct InstanceBuilder {
    std::string app_name;
    std::string engine_name;
    uint32_t api_version;
    uint32_t app_version;
    uint32_t engine_version;
    VkInstanceCreateFlags instance_create_flags;
    std::vector<std::string> instance_extensions;
    // DEBUGGING
    std::vector<std::string> validation_layers;
    // debug messenger ext
    VkDebugUtilsMessengerCreateInfoEXT messenger_create_info;
    // layer settings ext
    VkLayerSettingsCreateInfoEXT layer_settings_create_info;
    std::vector<VkLayerSettingEXT> layer_settings;
    // validation features ext
    VkValidationFeaturesEXT validation_features;
    std::vector<VkValidationFeatureEnableEXT> enabled_validation_features;
    std::vector<VkValidationFeatureDisableEXT> disabled_validation_features;
    // validation flags ext
    std::vector<VkValidationCheckEXT> disabled_validation_checks;
    VkValidationFlagsEXT validation_flags;
};

[[nodiscard]] VkInstance instance_builder_create_instance(InstanceBuilder* builder);

// required
void instance_builder_set_names(InstanceBuilder* builder, std::string_view app_name, std::string_view engine_name);

// required
void instance_builder_set_versions(InstanceBuilder* builder,uint32_t api, uint32_t app, uint32_t engine);

void instance_builder_set_instance_flags(InstanceBuilder* builder,VkInstanceCreateFlags instance_create_flags);

void instance_builder_set_instance_extensions(InstanceBuilder* builder, std::span<const char*> instance_extensions);

void instance_builder_set_validation_layers(InstanceBuilder* builder, std::span<const char*> validation_layers);

void instance_builder_set_debug_messenger(InstanceBuilder* builder, VkDebugUtilsMessageSeverityFlagsEXT severity, VkDebugUtilsMessageTypeFlagsEXT type, PFN_vkDebugUtilsMessengerCallbackEXT callback, void* user_data);

void instance_builder_set_layer_settings(InstanceBuilder* builder, std::span<VkLayerSettingEXT> layer_settings);

void instance_builder_set_validation_features(InstanceBuilder* builder, std::span<VkValidationFeatureEnableEXT> enabled_features, std::span<VkValidationFeatureDisableEXT> disabled_features);

void instance_builder_set_validation_flags(InstanceBuilder* builder,std::span<VkValidationCheckEXT> disabled_validation_checks);