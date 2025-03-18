#include <iostream>
#include "vk_lib/core.h"


int main() {
    InstanceBuilder instance_builder{};
    instance_builder_set_names(&instance_builder, "app name", "engine name");
    instance_builder_set_versions(&instance_builder, VK_API_VERSION_1_3, VK_MAKE_VERSION(1, 0, 0), VK_MAKE_VERSION(1, 0, 0));

    VkInstance instance;
    VkResult result = instance_builder_instance_create(&instance_builder, &instance);
    std::vector<PhysicalDevice> physical_devices;
    result = physical_device_enumerate_devices(instance, &physical_devices);
}