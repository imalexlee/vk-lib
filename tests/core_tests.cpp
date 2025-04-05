#include <gtest/gtest.h>
#include <vector>
#include <vk_lib/commands.h>
#include <vk_lib/core.h>

class CoreTestsFixture : public testing::Test {
  public:
    CoreTestsFixture() {

        auto app_info = vk_lib::application_info("My App",                        // application name
                                                 "My Engine",                     // engine name
                                                 VK_API_VERSION_1_3,              // Vulkan API version
                                                 VK_MAKE_API_VERSION(0, 1, 0, 0), // app version
                                                 VK_MAKE_API_VERSION(0, 1, 0, 0)  // engine version
        );

        // Setup instance extensions and layers
        std::array extensions = {"VK_KHR_surface"};
        std::array layers     = {"VK_LAYER_KHRONOS_validation"};
        // Create instance
        auto instance_info = vk_lib::instance_create_info(&app_info, layers, extensions);

        vk_lib::create_instance_with_entrypoints(&instance_info, &basic_instance);

        std::vector<VkPhysicalDevice> physical_devices{};
        vk_lib::enumerate_physical_devices(basic_instance, &physical_devices);

        assert(!physical_devices.empty());

        physical_device = physical_devices[0];
    }

    ~CoreTestsFixture() override {}

  protected:
    VkInstance       basic_instance{nullptr};
    VkPhysicalDevice physical_device{nullptr};
};

TEST_F(CoreTestsFixture, createBasicInstance) { ASSERT_NE(basic_instance, nullptr); }

TEST_F(CoreTestsFixture, createDevice) {
    // First, set up queue creation info (as shown in previous example)
    std::array queue_priorities = {1.0f};
    auto       queue_info       = vk_lib::device_queue_create_info(0, // family index
                                                                   1, // queue count
                                                                   queue_priorities);
    // Specify required device extensions
    std::array device_extensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    // Set up device features
    VkPhysicalDeviceFeatures device_features{};
    device_features.samplerAnisotropy = VK_TRUE;

    std::array queue_create_infos = {queue_info};
    auto       device_info        = vk_lib::device_create_info(queue_create_infos, device_extensions, &device_features);

    // Create logical device
    VkDevice device;
    if (vk_lib::create_device_with_entrypoints(physical_device, &device_info, &device) != VK_SUCCESS) {
        // Handle Error
    }
}
