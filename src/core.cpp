#include <vk_lib/core.h>

namespace vk_lib {

VkApplicationInfo application_info(const char* app_name, const char* engine_name, uint32_t api_version, uint32_t app_version,
                                   uint32_t engine_version) {
    VkApplicationInfo application_info{};
    application_info.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    application_info.pApplicationName   = app_name;
    application_info.pEngineName        = engine_name;
    application_info.apiVersion         = api_version;
    application_info.applicationVersion = app_version;
    application_info.engineVersion      = engine_version;

    return application_info;
}

VkInstanceCreateInfo instance_create_info(const VkApplicationInfo* app_info, std::span<const char*> layers,
                                          std::span<const char*> instance_extensions, VkInstanceCreateFlags flags, const void* pNext) {
    VkInstanceCreateInfo instance_create_info{};
    instance_create_info.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_create_info.pApplicationInfo        = app_info;
    instance_create_info.enabledLayerCount       = layers.size();
    instance_create_info.ppEnabledLayerNames     = layers.data();
    instance_create_info.enabledExtensionCount   = instance_extensions.size();
    instance_create_info.ppEnabledExtensionNames = instance_extensions.data();
    instance_create_info.flags                   = flags;
    instance_create_info.pNext                   = pNext;

    return instance_create_info;
}

// VkResult enumerate_instance_layer_properties(std::vector<VkLayerProperties>* layer_properties) {
//     uint32_t       layer_count = 0;
//     const VkResult result      = vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
//     if (result != VK_SUCCESS) {
//         return result;
//     }
//     layer_properties->clear();
//     layer_properties->resize(layer_count);
//     return vkEnumerateInstanceLayerProperties(&layer_count, layer_properties->data());
// }

// VkResult enumerate_physical_devices(VkInstance instance, std::vector<VkPhysicalDevice>* physical_devices) {
//     uint32_t       physical_device_count = 0;
//     const VkResult result                = vkEnumeratePhysicalDevices(instance, &physical_device_count, nullptr);
//     if (result != VK_SUCCESS) {
//         return result;
//     }
//     physical_devices->clear();
//     physical_devices->resize(physical_device_count);
//     return vkEnumeratePhysicalDevices(instance, &physical_device_count, physical_devices->data());
// }

// std::vector<VkQueueFamilyProperties> get_physical_device_queue_family_properties(VkPhysicalDevice physical_device) {
//     std::vector<VkQueueFamilyProperties> queue_family_properties;
//     uint32_t                             family_property_count;
//
//     vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &family_property_count, nullptr);
//     queue_family_properties.resize(family_property_count);
//     vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &family_property_count, queue_family_properties.data());
//
//     return queue_family_properties;
// }

VkDeviceQueueCreateInfo device_queue_create_info(uint32_t family_index, std::span<const float> queue_priorities, VkDeviceQueueCreateFlags flags,
                                                 const void* pNext) {
    VkDeviceQueueCreateInfo queue_create_info{};
    queue_create_info.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.queueCount       = queue_priorities.size();
    queue_create_info.queueFamilyIndex = family_index;
    queue_create_info.pQueuePriorities = queue_priorities.data();
    queue_create_info.flags            = flags;
    queue_create_info.pNext            = pNext;

    return queue_create_info;
}

VkDeviceCreateInfo device_create_info(std::span<const VkDeviceQueueCreateInfo> queue_create_infos, std::span<const char*> device_extensions,
                                      const VkPhysicalDeviceFeatures* features, const void* pNext) {
    VkDeviceCreateInfo device_create_info{};
    device_create_info.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_create_info.pQueueCreateInfos       = queue_create_infos.data();
    device_create_info.queueCreateInfoCount    = queue_create_infos.size();
    device_create_info.ppEnabledExtensionNames = device_extensions.data();
    device_create_info.enabledExtensionCount   = device_extensions.size();
    device_create_info.pEnabledFeatures        = features;
    device_create_info.pNext                   = pNext;

    return device_create_info;
}

// VkResult create_device_with_entrypoints(VkPhysicalDevice physical_device, const VkDeviceCreateInfo* device_create_info, VkDevice* device) {
//     const VkResult result = vkCreateDevice(physical_device, device_create_info, nullptr, device);
//     if (result != VK_SUCCESS) {
//         return result;
//     }
//
//     volkLoadDevice(*device);
//
//     return VK_SUCCESS;
// }

// VkResult enumerate_device_extension_properties(VkPhysicalDevice physical_device, std::vector<VkExtensionProperties> extension_properties,
//                                                const char* layer_name) {
//     uint32_t       property_count = 0;
//     const VkResult result         = vkEnumerateDeviceExtensionProperties(physical_device, layer_name, &property_count, nullptr);
//     if (result != VK_SUCCESS) {
//         return result;
//     }
//     extension_properties.resize(property_count);
//     return vkEnumerateDeviceExtensionProperties(physical_device, layer_name, &property_count, extension_properties.data());
// }

} // namespace vk_lib