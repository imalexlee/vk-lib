#include <vk_lib/core.h>


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

void instance_builder_set_layers(InstanceBuilder* builder, std::span<const char*> layers) {
    builder->layers.reserve(layers.size());
    for (const char* layer : layers) {
        builder->layers.emplace_back(layer);
    }
}

void instance_builder_set_instance_pNext(InstanceBuilder* builder, const void* pNext) {
    builder->instance_pNext = pNext;

}

void instance_builder_clear(InstanceBuilder* builder) {
    *builder = InstanceBuilder{};
}

VkResult instance_builder_instance_create(const InstanceBuilder* builder, VkInstance* instance) {

    VkResult result = volkInitialize();
    if (result != VK_SUCCESS) {
        return result;
    }

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
    instance_create_info.pNext            = builder->instance_pNext;

    std::vector<const char*> instance_extensions;
    instance_extensions.reserve(builder->instance_extensions.size());
    for (const std::string& extension : builder->instance_extensions) {
        instance_extensions.push_back(extension.c_str());
    }
    instance_create_info.ppEnabledExtensionNames = instance_extensions.data();
    instance_create_info.enabledExtensionCount   = instance_extensions.size();

    std::vector<const char*> validation_layers;
    validation_layers.reserve(builder->layers.size());
    for (const std::string& layer : builder->layers) {
        validation_layers.push_back(layer.c_str());
    }
    instance_create_info.ppEnabledLayerNames = validation_layers.data();
    instance_create_info.enabledLayerCount   = validation_layers.size();

    result = vkCreateInstance(&instance_create_info, nullptr, instance);
    if (result != VK_SUCCESS) {
        return result;
    }

    volkLoadInstanceOnly(*instance);

    return VK_SUCCESS;
}

void instance_destroy(VkInstance instance) {
    vkDestroyInstance(instance, nullptr);
}

VkResult instance_enumerate_layer_properties(std::vector<VkLayerProperties>* layer_properties) {
    uint32_t layer_count;
    VkResult result = vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
    if (result != VK_SUCCESS) {
        return result;
    }
    layer_properties->clear();
    layer_properties->resize(layer_count);
    return vkEnumerateInstanceLayerProperties(&layer_count, layer_properties->data());
}


VkResult physical_device_enumerate_devices(VkInstance instance, std::vector<VkPhysicalDevice>* physical_devices) {
    uint32_t physical_device_count = 0;
    const VkResult result          = vkEnumeratePhysicalDevices(instance, &physical_device_count, nullptr);
    if (result != VK_SUCCESS) {
        return result;
    }

    physical_devices->clear();
    physical_devices->resize(physical_device_count);
    return vkEnumeratePhysicalDevices(instance, &physical_device_count, physical_devices->data());
}

VkPhysicalDeviceProperties physical_device_get_properties(VkPhysicalDevice physical_device) {
    VkPhysicalDeviceProperties properties{};
    vkGetPhysicalDeviceProperties(physical_device, &properties);
    return properties;
}


VkPhysicalDeviceFeatures physical_device_get_features(VkPhysicalDevice physical_device) {
    VkPhysicalDeviceFeatures physical_device_features{};
    vkGetPhysicalDeviceFeatures(physical_device, &physical_device_features);
    return physical_device_features;
}

VkPhysicalDeviceFeatures2 physical_device_get_features2(VkPhysicalDevice physical_device, void* extended_feature_chain) {
    // must pass in the pNext chain of features to query for
    VkPhysicalDeviceFeatures2 physical_device_features{};
    physical_device_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
    physical_device_features.pNext = extended_feature_chain;
    vkGetPhysicalDeviceFeatures2(physical_device, &physical_device_features);
    return physical_device_features;
}

std::vector<VkQueueFamilyProperties> physical_device_enumerate_queue_families(VkPhysicalDevice physical_device) {
    std::vector<VkQueueFamilyProperties> queue_family_properties;
    uint32_t family_property_count;

    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &family_property_count, nullptr);
    queue_family_properties.resize(family_property_count);
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &family_property_count, queue_family_properties.data());

    return queue_family_properties;
}

void logical_device_builder_set_device_features_1(LogicalDeviceBuilder* builder, VkPhysicalDeviceFeatures features, void* extended_feature_chain) {
    builder->physical_device_features2.reset();
    builder->physical_device_features = features;
    builder->extended_feature_chain   = extended_feature_chain;
}

void logical_device_builder_set_device_features_2(LogicalDeviceBuilder* builder, VkPhysicalDeviceFeatures2 features_2) {
    // if using VkPhysicalDeviceFeatures2, pEnabledFeatures in vkDeviceCreateInfo must be null
    // NOTE: if using VkPhysicalDeviceFeatures2, put all feature extensions (including vulkan 1.1, 1.2, etc.) in the
    // pNext chain of VkPhysicalDeviceFeatures2
    builder->physical_device_features.reset();
    builder->physical_device_features2 = features_2;
    builder->extended_feature_chain    = nullptr;
}

void logical_device_builder_queue_create(LogicalDeviceBuilder* builder, uint32_t queue_family_index, float priority) {
    builder->queue_family_creation_map[queue_family_index]++;
    builder->queue_priorities_map[queue_family_index].push_back(priority);
}

void logical_device_builder_set_device_extensions(LogicalDeviceBuilder* builder, std::span<const char*> device_extensions) {
    builder->device_extensions.reserve(device_extensions.size());
    for (const char* extension : device_extensions) {
        builder->device_extensions.emplace_back(extension);
    }
}

VkResult logical_device_builder_device_create(LogicalDeviceBuilder* builder, VkPhysicalDevice physical_device, VkDevice* device) {
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
    if (builder->physical_device_features.has_value()) {
        device_create_info.pEnabledFeatures = &builder->physical_device_features.value();
        device_create_info.pNext            = builder->extended_feature_chain;
    } else if (builder->physical_device_features2.has_value()) {
        device_create_info.pNext = &builder->physical_device_features2.value();
    }

    const VkResult result = vkCreateDevice(physical_device, &device_create_info, nullptr, device);
    if (result != VK_SUCCESS) {
        return result;
    }

    volkLoadDevice(*device);

    return VK_SUCCESS;
}

void logical_device_destroy(VkDevice device) {
    vkDestroyDevice(device, nullptr);
}

VkQueue queue_get(VkDevice device, uint32_t queue_family_index, uint32_t queue_index) {
    VkQueue queue;
    vkGetDeviceQueue(device, queue_family_index, queue_index, &queue);
    return queue;
}
