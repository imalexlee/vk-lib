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

void instance_builder_set_extensions(InstanceBuilder* builder, std::span<const char*> instance_extensions) {
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

void instance_builder_set_instance_pNext(InstanceBuilder* builder, const void* pNext) { builder->instance_pNext = pNext; }

void instance_builder_clear(InstanceBuilder* builder) { *builder = InstanceBuilder{}; }

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

    std::vector<const char*> layers;
    layers.reserve(builder->layers.size());
    for (const std::string& layer : builder->layers) {
        layers.push_back(layer.c_str());
    }
    instance_create_info.ppEnabledLayerNames = layers.data();
    instance_create_info.enabledLayerCount   = layers.size();

    result = vkCreateInstance(&instance_create_info, nullptr, instance);
    if (result != VK_SUCCESS) {
        return result;
    }

    volkLoadInstanceOnly(*instance);

    return VK_SUCCESS;
}

void instance_destroy(VkInstance instance) { vkDestroyInstance(instance, nullptr); }

VkResult instance_enumerate_layer_properties(std::vector<VkLayerProperties>* layer_properties) {
    uint32_t       layer_count;
    const VkResult result = vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
    if (result != VK_SUCCESS) {
        return result;
    }
    layer_properties->clear();
    layer_properties->resize(layer_count);
    return vkEnumerateInstanceLayerProperties(&layer_count, layer_properties->data());
}

VkResult physical_device_enumerate_devices(VkInstance instance, std::vector<VkPhysicalDevice>* physical_devices) {
    uint32_t       physical_device_count = 0;
    const VkResult result                = vkEnumeratePhysicalDevices(instance, &physical_device_count, nullptr);
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

std::vector<VkQueueFamilyProperties> physical_device_enumerate_queue_families(VkPhysicalDevice physical_device) {
    std::vector<VkQueueFamilyProperties> queue_family_properties;
    uint32_t                             family_property_count;

    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &family_property_count, nullptr);
    queue_family_properties.resize(family_property_count);
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &family_property_count, queue_family_properties.data());

    return queue_family_properties;
}

VkQueue queue_get(VkDevice device, uint32_t queue_family_index, uint32_t queue_index) {
    VkQueue queue;
    vkGetDeviceQueue(device, queue_family_index, queue_index, &queue);
    return queue;
}

VkDeviceQueueCreateInfo queue_create_info_create(uint32_t family_index, uint32_t queue_count, std::span<float> queue_priorities,
                                                 VkDeviceQueueCreateFlags flags, const void* pNext) {
    VkDeviceQueueCreateInfo queue_create_info{};
    queue_create_info.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.queueCount       = queue_count;
    queue_create_info.queueFamilyIndex = family_index;
    queue_create_info.pQueuePriorities = queue_priorities.data();
    queue_create_info.flags            = flags;
    queue_create_info.pNext            = pNext;

    return queue_create_info;
}

void device_destroy(VkDevice device) { vkDestroyDevice(device, nullptr); }

VkResult device_create(VkPhysicalDevice physical_device, std::span<VkDeviceQueueCreateInfo> queue_create_infos, VkDevice* device,
                       std::span<const char*> extension_names, const VkPhysicalDeviceFeatures* enabled_features, const void* pNext) {

    VkDeviceCreateInfo device_create_info{};
    device_create_info.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_create_info.pQueueCreateInfos       = queue_create_infos.data();
    device_create_info.queueCreateInfoCount    = queue_create_infos.size();
    device_create_info.ppEnabledExtensionNames = extension_names.data();
    device_create_info.enabledExtensionCount   = extension_names.size();
    device_create_info.pEnabledFeatures        = enabled_features;
    device_create_info.pNext                   = pNext;

    const VkResult result = vkCreateDevice(physical_device, &device_create_info, nullptr, device);
    if (result != VK_SUCCESS) {
        return result;
    }

    volkLoadDevice(*device);

    return VK_SUCCESS;
}

VkResult device_enumerate_extension_properties(VkPhysicalDevice physical_device, std::vector<VkExtensionProperties> extension_properties,
                                               std::string_view layer_name) {
    const char*    layer_str      = layer_name.empty() ? nullptr : layer_name.data();
    uint32_t       property_count = 0;
    const VkResult result         = vkEnumerateDeviceExtensionProperties(physical_device, layer_str, &property_count, nullptr);
    if (result != VK_SUCCESS) {
        return result;
    }
    extension_properties.resize(property_count);
    return vkEnumerateDeviceExtensionProperties(physical_device, layer_str, &property_count, extension_properties.data());
}

VkPhysicalDeviceFeatures2KHR physical_device_get_features_2(VkPhysicalDevice physical_device, void* extended_feature_chain) {
    // must pass in the pNext chain of features to query for
    VkPhysicalDeviceFeatures2KHR physical_device_features{};
    physical_device_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2_KHR;
    physical_device_features.pNext = extended_feature_chain;
    vkGetPhysicalDeviceFeatures2KHR(physical_device, &physical_device_features);
    return physical_device_features;
}
