#include <iostream>
#include "vk_lib/core.h"

#include <assert.h>


int main(){
  std::cout << "Hello there" << std::endl;
    InstanceBuilder instance_builder{};
    instance_builder_set_names(&instance_builder, "app name", "engine name");
    instance_builder_set_versions(&instance_builder, VK_API_VERSION_1_3, VK_MAKE_VERSION(1, 0, 0),VK_MAKE_VERSION(1, 0, 0 ));
    VkInstance instance = instance_builder_create_instance(&instance_builder);
}