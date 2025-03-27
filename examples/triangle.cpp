#include "vk_lib/core.h"

#include <vk_lib/presentation.h>
#include <vulkan/vk_enum_string_helper.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#ifdef NDEBUG
constexpr bool enable_validation_layers = false;
#else
constexpr bool enable_validation_layers = true;
#endif

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
    uint32_t         graphics_present_queue_family{};
};

VkInstance create_instance() {
    std::array      extensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    InstanceBuilder instance_builder;
    instance_builder_set_names(&instance_builder, "hello triangle", "engine name");
    instance_builder_set_versions(&instance_builder, VK_API_VERSION_1_3);
    instance_builder_set_extensions(&instance_builder, extensions);
    // Only enable validation layers in debug builds
    if constexpr (enable_validation_layers) {
        std::array layers = {"VK_LAYER_KHRONOS_validation"};
        instance_builder_set_layers(&instance_builder, layers);
    }
    VkInstance instance;
    VK_CHECK(instance_builder_instance_create(&instance_builder, &instance));
    return instance;
}

VkPhysicalDevice select_a_physical_device(VkInstance instance) {
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

int main() {
    // TODO: obtain a surface from glfw
    VkContext vk_context{};
    vk_context.instance                      = create_instance();
    vk_context.physical_device               = select_a_physical_device(vk_context.instance);
    vk_context.graphics_present_queue_family = select_queue_family(vk_context.physical_device, surface);

    std::vector<VkQueueFamilyProperties> queue_family_properties = physical_device_enumerate_queue_families(vk_context.physical_device);
}