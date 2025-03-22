#include <iostream>
#include "vk_lib/core.h"

#include <vk_lib/shader_data.h>


int main() {
    push_constant_range_create(VK_SHADER_STAGE_ALL, 1, 1);
}