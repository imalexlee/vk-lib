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
    instance_create_info.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_create_info.pApplicationInfo        = &app_info;
    instance_create_info.flags                   = builder->instance_create_flags;

    std::vector<const char*> instance_extensions;
    instance_extensions.reserve(builder->instance_extensions.size());
    for (const std::string& extension: builder->instance_extensions) {
        instance_extensions.push_back(extension.c_str());
    }
    instance_create_info.ppEnabledExtensionNames = instance_extensions.data();
    instance_create_info.enabledExtensionCount   = instance_extensions.size();


    std::vector<const char*> validation_layers;
    validation_layers.reserve(builder->validation_layers.size());
    for (const std::string& layer: builder->validation_layers) {
        validation_layers.push_back(layer.c_str());
    }
    instance_create_info.ppEnabledLayerNames = validation_layers.data();
    instance_create_info.enabledLayerCount = validation_layers.size();

    for (const std::string& extension: builder->instance_extensions) {
        if (extension == VK_EXT_DEBUG_UTILS_EXTENSION_NAME ) {
            builder->messenger_create_info.pNext =  instance_create_info.pNext;
            instance_create_info.pNext = &builder->messenger_create_info;
        }
        if (extension == VK_EXT_LAYER_SETTINGS_EXTENSION_NAME) {
            builder->layer_settings_create_info.pNext =  instance_create_info.pNext;
            instance_create_info.pNext = &builder->layer_settings_create_info;
        }
        if (extension == VK_EXT_VALIDATION_FEATURES_EXTENSION_NAME) {
            builder->validation_features.pNext =  instance_create_info.pNext;
            instance_create_info.pNext = &builder->validation_features;
        }
        if (extension== VK_EXT_VALIDATION_FLAGS_EXTENSION_NAME) {
            builder->validation_flags.pNext =  instance_create_info.pNext;
            instance_create_info.pNext = &builder->validation_flags;
        }
    }

    VkInstance instance;
    VK_CHECK(vkCreateInstance(&instance_create_info, nullptr, &instance));
    return instance;

}

void instance_builder_set_names(InstanceBuilder* builder, std::string_view app_name, std::string_view engine_name) {
    builder->app_name = app_name;
    builder->engine_name = engine_name;
}

void instance_builder_set_versions(InstanceBuilder* builder, uint32_t api, uint32_t app, uint32_t engine) {
    builder->api_version = api;
    builder->app_version = app;
    builder->engine_version = engine;
}

void instance_builder_set_instance_flags(InstanceBuilder* builder,VkInstanceCreateFlags instance_create_flags) {
    builder->instance_create_flags = instance_create_flags;
}

void instance_builder_set_instance_extensions(InstanceBuilder* builder, std::span<const char*> instance_extensions) {
    builder->instance_extensions.reserve(instance_extensions.size());
    for (const char* extension: instance_extensions) {
        builder->instance_extensions.push_back(extension);
    }
}

void instance_builder_set_validation_layers(InstanceBuilder* builder, std::span<const char*> validation_layers) {
    builder->validation_layers.reserve(validation_layers.size());
    for (const char* layer: validation_layers) {
        builder->validation_layers.push_back(layer);
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
    messenger_create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    messenger_create_info.messageSeverity = severity;
    messenger_create_info.messageType = type;
    messenger_create_info.pfnUserCallback = callback;
    messenger_create_info.pUserData = user_data;

    builder->messenger_create_info = messenger_create_info;
    builder->instance_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
}

void instance_builder_set_layer_settings(InstanceBuilder* builder, std::span<VkLayerSettingEXT> layer_settings) {
    builder->layer_settings.reserve(layer_settings.size());
    for (const VkLayerSettingEXT& setting: layer_settings) {
        builder->layer_settings.push_back(setting);
    }

    VkLayerSettingsCreateInfoEXT layer_settings_create_info{};
    layer_settings_create_info.sType = VK_STRUCTURE_TYPE_LAYER_SETTINGS_CREATE_INFO_EXT;
    layer_settings_create_info.pSettings = builder->layer_settings.data();
    layer_settings_create_info.settingCount = builder->layer_settings.size();

    builder->instance_extensions.push_back(VK_EXT_LAYER_SETTINGS_EXTENSION_NAME);
}

void instance_builder_set_validation_features(InstanceBuilder* builder, std::span<VkValidationFeatureEnableEXT> enabled_features, std::span<VkValidationFeatureDisableEXT> disabled_features) {
    VkValidationFeaturesEXT validation_features{};
    validation_features.sType                          = VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT;
    validation_features.pEnabledValidationFeatures     = enabled_features.data();
    validation_features.enabledValidationFeatureCount  = enabled_features.size();
    validation_features.pDisabledValidationFeatures    = disabled_features.data();
    validation_features.disabledValidationFeatureCount = disabled_features.size();

    builder->validation_features = validation_features;
    builder->instance_extensions.push_back(VK_EXT_VALIDATION_FEATURES_EXTENSION_NAME);
}

void instance_builder_set_validation_flags(InstanceBuilder* builder,std::span<VkValidationCheckEXT> disabled_validation_checks) {
    builder->disabled_validation_checks.reserve(disabled_validation_checks.size());
    for (const VkValidationCheckEXT& check: disabled_validation_checks) {
        builder->disabled_validation_checks.push_back(check);
    }

    VkValidationFlagsEXT validation_flags;
    validation_flags.sType = VK_STRUCTURE_TYPE_VALIDATION_FLAGS_EXT;
    validation_flags.pDisabledValidationChecks = builder->disabled_validation_checks.data();
    validation_flags.disabledValidationCheckCount = builder->disabled_validation_checks.size();

    builder->instance_extensions.push_back(VK_EXT_VALIDATION_FLAGS_EXTENSION_NAME);
}
