#include "vk_lib/core.h"

#include <vk_lib/presentation.h>
#include <vulkan/vk_enum_string_helper.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define VK_CHECK(x)                                                                                                                                  \
    do {                                                                                                                                             \
        VkResult err = x;                                                                                                                            \
        if (err) {                                                                                                                                   \
            std::cerr << "Detected Vulkan error: " << string_VkResult(err) << std::endl;                                                             \
            abort();                                                                                                                                 \
        }                                                                                                                                            \
    } while (0)

struct VkContext {
    VkInstance       instance{};
    VkPhysicalDevice physical_device{};
    VkSurfaceKHR     surface{};
    GLFWwindow*      window{};
    uint32_t         graphics_present_queue_family{};
};

VkInstance create_instance() {
    uint32_t                 glfw_extension_count = 0;
    bool                     vulkan_supported     = glfwVulkanSupported();
    const char**             glfw_extensions      = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
    std::vector<const char*> extensions{};
    extensions.reserve(glfw_extension_count + 3);
    extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    // extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
    for (uint32_t i = 0; i < glfw_extension_count; i++) {
        extensions.push_back(glfw_extensions[i]);
    }
    InstanceBuilder instance_builder;
    instance_builder_set_names(&instance_builder, "hello triangle", "engine name");
    instance_builder_set_versions(&instance_builder, VK_API_VERSION_1_3);
#ifdef __APPLE__
    extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
    instance_builder_set_instance_flags(&instance_builder, VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR);
#endif
    instance_builder_set_extensions(&instance_builder, extensions);
#ifndef NDEBUG
    std::array layers = {"VK_LAYER_KHRONOS_validation"};
    instance_builder_set_layers(&instance_builder, layers);
#endif
    VkInstance instance;
    VK_CHECK(instance_builder_instance_create(&instance_builder, &instance));
    return instance;
}

VkPhysicalDevice select_physical_device(VkInstance instance) {
    // Find a device that supports vulkan 1.3. Prefer discrete GPU's
    std::vector<VkPhysicalDevice> physical_devices;
    VK_CHECK(physical_device_enumerate_devices(instance, &physical_devices));
    VkPhysicalDevice chosen_device = nullptr;
    for (const VkPhysicalDevice& physical_device : physical_devices) {
        const VkPhysicalDeviceProperties properties = physical_device_get_properties(physical_device);
        if (properties.apiVersion == VK_API_VERSION_1_3) {
            chosen_device = physical_device;
            if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
                return chosen_device;
            }
        }
    }
    if (chosen_device == nullptr) {
        std::cerr << "Could not find a suitable physical device" << std::endl;
        std::abort();
    }
    return chosen_device;
}

uint32_t select_queue_family(VkPhysicalDevice physical_device, VkSurfaceKHR surface) {
    const std::vector<VkQueueFamilyProperties> queue_family_properties = physical_device_enumerate_queue_families(physical_device);
    // Find a queue family with both graphics and presentation capabilities
    for (uint32_t i = 0; i < queue_family_properties.size(); i++) {
        const VkQueueFamilyProperties* family_properties = &queue_family_properties[i];
        if (family_properties->queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            bool present_supported = false;
            physical_device_get_surface_support(physical_device, surface, i, &present_supported);
            if (present_supported) {
                return i;
            }
        }
    }
    std::cerr << "Could not find a suitable queue family" << std::endl;
    std::abort();
}

VkDevice create_logical_device(VkPhysicalDevice physical_device, uint32_t queue_family) {
    VkDevice                device;
    std::array              queue_priorities   = {1.f};
    VkDeviceQueueCreateInfo queue_create_info  = queue_create_info_create(queue_family, 1, queue_priorities);
    std::array              queue_create_infos = {queue_create_info};

    std::array device_extensions = {VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME, VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME};

    VkPhysicalDeviceVulkan13Features vk_1_3_features{};
    vk_1_3_features.sType                                = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
    VkPhysicalDeviceFeatures2 physical_device_features_2 = physical_device_get_features_2(physical_device, &vk_1_3_features);

    if (vk_1_3_features.dynamicRendering == VK_FALSE || vk_1_3_features.synchronization2 == VK_FALSE) {
        std::cerr << "Required features are not supported by this device" << std::endl;
        std::abort();
    }
    // enable only dynamic rendering and sync 2 now, instead of all the supported vulkan 1.3 features
    vk_1_3_features                  = VkPhysicalDeviceVulkan13Features{VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES};
    vk_1_3_features.dynamicRendering = VK_TRUE;
    vk_1_3_features.synchronization2 = VK_TRUE;

    device_create(physical_device, queue_create_infos, &device, device_extensions, nullptr, &physical_device_features_2);
    return device;
}

int main() {
    VkContext vk_context{};

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    vk_context.window          = glfwCreateWindow(640, 480, "Hello Triangle", nullptr, nullptr);
    vk_context.instance        = create_instance();
    vk_context.physical_device = select_physical_device(vk_context.instance);
    VK_CHECK(glfwCreateWindowSurface(vk_context.instance, vk_context.window, nullptr, &vk_context.surface));
    vk_context.graphics_present_queue_family = select_queue_family(vk_context.physical_device, vk_context.surface);

    std::vector<VkQueueFamilyProperties> queue_family_properties = physical_device_enumerate_queue_families(vk_context.physical_device);
}