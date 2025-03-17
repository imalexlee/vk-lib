/*
* Utilities regarding memory management, images, buffers, and acceleration structures
*/

#pragma once
#include <vk_lib/common.h>

[[nodiscard]] VkImageSubresourceRange vk_image_subresource_range_create(
    VkImageAspectFlags aspect_flags,
    uint32_t base_array_layer,
    uint32_t array_layer_count,
    uint32_t base_mip_level,
    uint32_t mip_level_count);
