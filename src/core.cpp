#include <vk_lib/core.h>

VkInstance instance_builder_create_instance(InstanceBuilder* builder) {
    VkApplicationInfo app_info{};
    app_info.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pNext              = nullptr;
    app_info.pApplicationName   = builder->app_name.c_str();
    app_info.pEngineName        = builder->engine_name.c_str();
    app_info.engineVersion      = builder->engine_version;
    app_info.applicationVersion = builder->app_version;
    app_info.apiVersion         = builder->api_version;

    VkInstanceCreateInfo instance_create_info{};
    instance_create_info.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_create_info.pApplicationInfo = &app_info;
    instance_create_info.flags            = builder->instance_create_flags;

    std::vector<const char*> instance_extensions;
    instance_extensions.reserve(builder->instance_extensions.size());
    for (const std::string& extension : builder->instance_extensions) {
        instance_extensions.push_back(extension.c_str());
    }
    instance_create_info.ppEnabledExtensionNames = instance_extensions.data();
    instance_create_info.enabledExtensionCount   = instance_extensions.size();

    std::vector<const char*> validation_layers;
    validation_layers.reserve(builder->validation_layers.size());
    for (const std::string& layer : builder->validation_layers) {
        validation_layers.push_back(layer.c_str());
    }
    instance_create_info.ppEnabledLayerNames = validation_layers.data();
    instance_create_info.enabledLayerCount   = validation_layers.size();

    for (const std::string& extension : builder->instance_extensions) {
        if (extension == VK_EXT_DEBUG_UTILS_EXTENSION_NAME) {
            builder->messenger_create_info.pNext = instance_create_info.pNext;
            instance_create_info.pNext           = &builder->messenger_create_info;
        }
        if (extension == VK_EXT_LAYER_SETTINGS_EXTENSION_NAME) {
            builder->layer_settings_create_info.pNext = instance_create_info.pNext;
            instance_create_info.pNext                = &builder->layer_settings_create_info;
        }
        if (extension == VK_EXT_VALIDATION_FEATURES_EXTENSION_NAME) {
            builder->validation_features.pNext = instance_create_info.pNext;
            instance_create_info.pNext         = &builder->validation_features;
        }
        if (extension == VK_EXT_VALIDATION_FLAGS_EXTENSION_NAME) {
            builder->validation_flags.pNext = instance_create_info.pNext;
            instance_create_info.pNext      = &builder->validation_flags;
        }
    }

    VkInstance instance;
    VK_CHECK(vkCreateInstance(&instance_create_info, nullptr, &instance));
    return instance;

}

void instance_builder_set_names(InstanceBuilder* builder, std::string_view app_name, std::string_view engine_name) {
    builder->app_name    = app_name;
    builder->engine_name = engine_name;
}

void instance_builder_set_versions(InstanceBuilder* builder, uint32_t api, uint32_t app, uint32_t engine) {
    builder->api_version    = api;
    builder->app_version    = app;
    builder->engine_version = engine;
}

void instance_builder_set_instance_flags(InstanceBuilder* builder, VkInstanceCreateFlags instance_create_flags) {
    builder->instance_create_flags = instance_create_flags;
}

void instance_builder_set_instance_extensions(InstanceBuilder* builder, std::span<const char*> instance_extensions) {
    builder->instance_extensions.reserve(instance_extensions.size());
    for (const char* extension : instance_extensions) {
        builder->instance_extensions.emplace_back(extension);
    }
}

void instance_builder_set_validation_layers(InstanceBuilder* builder, std::span<const char*> validation_layers) {
    builder->validation_layers.reserve(validation_layers.size());
    for (const char* layer : validation_layers) {
        builder->validation_layers.emplace_back(layer);
    }
}

void instance_builder_set_debug_messenger(
    InstanceBuilder* builder,
    VkDebugUtilsMessageSeverityFlagsEXT severity,
    VkDebugUtilsMessageTypeFlagsEXT type,
    PFN_vkDebugUtilsMessengerCallbackEXT callback,
    void* user_data
    ) {
    // TODO: set a default callback if caller did not provide one
    VkDebugUtilsMessengerCreateInfoEXT messenger_create_info{};
    messenger_create_info.sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    messenger_create_info.messageSeverity = severity;
    messenger_create_info.messageType     = type;
    messenger_create_info.pfnUserCallback = callback;
    messenger_create_info.pUserData       = user_data;

    builder->messenger_create_info = messenger_create_info;
    builder->instance_extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
}

void instance_builder_set_layer_settings(InstanceBuilder* builder, std::span<VkLayerSettingEXT> layer_settings) {
    builder->layer_settings.reserve(layer_settings.size());
    for (const VkLayerSettingEXT& setting : layer_settings) {
        builder->layer_settings.push_back(setting);
    }

    VkLayerSettingsCreateInfoEXT layer_settings_create_info{};
    layer_settings_create_info.sType        = VK_STRUCTURE_TYPE_LAYER_SETTINGS_CREATE_INFO_EXT;
    layer_settings_create_info.pSettings    = builder->layer_settings.data();
    layer_settings_create_info.settingCount = builder->layer_settings.size();

    builder->layer_settings_create_info = layer_settings_create_info;
    builder->instance_extensions.emplace_back(VK_EXT_LAYER_SETTINGS_EXTENSION_NAME);
}

void instance_builder_set_validation_features(InstanceBuilder* builder, std::span<VkValidationFeatureEnableEXT> enabled_features,
                                              std::span<VkValidationFeatureDisableEXT> disabled_features) {
    VkValidationFeaturesEXT validation_features{};
    validation_features.sType                          = VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT;
    validation_features.pEnabledValidationFeatures     = enabled_features.data();
    validation_features.enabledValidationFeatureCount  = enabled_features.size();
    validation_features.pDisabledValidationFeatures    = disabled_features.data();
    validation_features.disabledValidationFeatureCount = disabled_features.size();

    builder->validation_features = validation_features;
    builder->instance_extensions.emplace_back(VK_EXT_VALIDATION_FEATURES_EXTENSION_NAME);
}

void instance_builder_set_validation_flags(InstanceBuilder* builder, std::span<VkValidationCheckEXT> disabled_validation_checks) {
    builder->disabled_validation_checks.reserve(disabled_validation_checks.size());
    for (const VkValidationCheckEXT& check : disabled_validation_checks) {
        builder->disabled_validation_checks.push_back(check);
    }

    VkValidationFlagsEXT validation_flags;
    validation_flags.sType                        = VK_STRUCTURE_TYPE_VALIDATION_FLAGS_EXT;
    validation_flags.pDisabledValidationChecks    = builder->disabled_validation_checks.data();
    validation_flags.disabledValidationCheckCount = builder->disabled_validation_checks.size();

    builder->validation_flags = validation_flags;
    builder->instance_extensions.emplace_back(VK_EXT_VALIDATION_FLAGS_EXTENSION_NAME);
}

std::vector<PhysicalDevice> enumerate_physical_devices(VkInstance instance) {
    std::vector<VkPhysicalDevice> vk_physical_devices;
    uint32_t physical_device_count;

    VK_CHECK(vkEnumeratePhysicalDevices(instance, &physical_device_count, nullptr));
    vk_physical_devices.resize(physical_device_count);
    VK_CHECK(vkEnumeratePhysicalDevices(instance, &physical_device_count, vk_physical_devices.data()));

    std::vector<PhysicalDevice> physical_devices;
    physical_devices.reserve(vk_physical_devices.size());
    for (VkPhysicalDevice vk_physical_device : vk_physical_devices) {
        VkPhysicalDeviceProperties properties{};
        vkGetPhysicalDeviceProperties(vk_physical_device, &properties);

        PhysicalDevice new_physical_device{};
        new_physical_device.physical_device            = vk_physical_device;
        new_physical_device.physical_device_properties = properties;
        physical_devices.push_back(new_physical_device);
    }

    return physical_devices;
}

std::vector<VkQueueFamilyProperties> enumerate_queue_families(VkPhysicalDevice physical_device) {
    std::vector<VkQueueFamilyProperties> queue_family_properties;
    uint32_t family_property_count;

    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &family_property_count, nullptr);
    queue_family_properties.resize(family_property_count);
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &family_property_count, queue_family_properties.data());

    return queue_family_properties;
}

void logical_device_builder_create_queue(LogicalDeviceBuilder* builder, uint32_t queue_family_index, float priority) {
    builder->queue_family_creation_map[queue_family_index]++;
    builder->queue_priorities_map[queue_family_index].push_back(priority);
}

void logical_device_builder_set_device_extensions(LogicalDeviceBuilder* builder, std::span<const char*> device_extensions) {
    builder->device_extensions.reserve(device_extensions.size());
    for (const char* extension : device_extensions) {
        builder->device_extensions.emplace_back(extension);
    }
}

VkDevice logical_device_builder_create_device(LogicalDeviceBuilder* builder, VkPhysicalDevice physical_device) {
    std::vector<const char*> device_extensions;
    device_extensions.reserve(builder->device_extensions.size());
    for (const std::string& extension : builder->device_extensions) {
        device_extensions.push_back(extension.c_str());
    }

    std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
    queue_create_infos.reserve(builder->queue_family_creation_map.size());
    for (const auto& [family_index, queue_count] : builder->queue_family_creation_map) {
        VkDeviceQueueCreateInfo new_queue_create_info{};
        new_queue_create_info.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        new_queue_create_info.queueCount       = queue_count;
        new_queue_create_info.queueFamilyIndex = family_index;
        new_queue_create_info.pQueuePriorities = builder->queue_priorities_map[family_index].data();

        queue_create_infos.push_back(new_queue_create_info);
    }

    VkDeviceCreateInfo device_create_info{};
    device_create_info.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_create_info.pQueueCreateInfos       = queue_create_infos.data();
    device_create_info.queueCreateInfoCount    = queue_create_infos.size();
    device_create_info.ppEnabledExtensionNames = device_extensions.data();
    device_create_info.enabledExtensionCount   = device_extensions.size();

    VkDevice device;
    VK_CHECK(vkCreateDevice(physical_device, &device_create_info, nullptr, &device));

    return device;
}