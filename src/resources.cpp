#include <vk_lib/resources.h>


VkImageSubresourceRange vk_image_subresource_range_create(
    VkImageAspectFlags aspect_flags,
    uint32_t base_array_layer,
    uint32_t array_layer_count,
    uint32_t base_mip_level,
    uint32_t mip_level_count
    ) {
    VkImageSubresourceRange subresource_range{};
    subresource_range.aspectMask     = aspect_flags;
    subresource_range.baseArrayLayer = base_array_layer;
    subresource_range.layerCount     = array_layer_count;
    subresource_range.baseMipLevel   = base_mip_level;
    subresource_range.levelCount     = mip_level_count;

    return subresource_range;
}
