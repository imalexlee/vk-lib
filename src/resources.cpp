#include <vk_lib/resources.h>

namespace vk_lib {

VkOffset2D offset_2d(int32_t x, int32_t y) {
    VkOffset2D offset_2d{};
    offset_2d.x = x;
    offset_2d.y = y;

    return offset_2d;
}

VkOffset3D offset_3d(int32_t x, int32_t y, int32_t z) {
    VkOffset3D offset_3d{};
    offset_3d.x = x;
    offset_3d.y = y;
    offset_3d.z = z;

    return offset_3d;
}

VkExtent2D extent_2d(uint32_t width, uint32_t height) {
    VkExtent2D extent_2d{};
    extent_2d.width  = width;
    extent_2d.height = height;

    return extent_2d;
}

VkExtent3D extent_3d(uint32_t width, uint32_t height, uint32_t depth) {
    VkExtent3D extent_3d{};
    extent_3d.width  = width;
    extent_3d.height = height;
    extent_3d.depth  = depth;

    return extent_3d;
}

VkRect2D rect_2d(VkExtent2D extent, VkOffset2D offset) {
    VkRect2D rect_2d{};
    rect_2d.extent = extent;
    rect_2d.offset = offset;

    return rect_2d;
}

VkImageCreateInfo image_create_info(VkFormat format, VkImageUsageFlags usage, VkExtent3D extent, VkImageLayout initial_layout, uint32_t mip_levels,
                                    uint32_t array_layers, VkSampleCountFlagBits samples, VkImageType type, VkImageCreateFlags flags,
                                    VkSharingMode sharing_mode, std::span<const uint32_t> queue_family_indices, VkImageTiling tiling,
                                    const void* pNext) {
    VkImageCreateInfo image_create_info{};
    image_create_info.sType                 = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_create_info.format                = format;
    image_create_info.flags                 = flags;
    image_create_info.imageType             = type;
    image_create_info.extent                = extent;
    image_create_info.mipLevels             = mip_levels;
    image_create_info.arrayLayers           = array_layers;
    image_create_info.samples               = samples;
    image_create_info.tiling                = tiling;
    image_create_info.usage                 = usage;
    image_create_info.sharingMode           = sharing_mode;
    image_create_info.queueFamilyIndexCount = queue_family_indices.size();
    image_create_info.pQueueFamilyIndices   = queue_family_indices.data();
    image_create_info.initialLayout         = initial_layout;
    image_create_info.pNext                 = pNext;

    return image_create_info;
}

VkImageSubresourceRange image_subresource_range(VkImageAspectFlags aspect_flags, uint32_t mip_level_count, uint32_t base_mip_level,
                                                uint32_t array_layer_count, uint32_t base_array_layer) {
    VkImageSubresourceRange subresource_range{};
    subresource_range.aspectMask     = aspect_flags;
    subresource_range.baseArrayLayer = base_array_layer;
    subresource_range.layerCount     = array_layer_count;
    subresource_range.baseMipLevel   = base_mip_level;
    subresource_range.levelCount     = mip_level_count;

    return subresource_range;
}

VkImageViewCreateInfo image_view_create_info(VkFormat format, VkImage image, const VkImageSubresourceRange* subresource_range,
                                             VkImageViewType view_type, VkImageCreateFlags flags, const VkComponentMapping* component_mapping,
                                             const void* pNext) {
    VkImageViewCreateInfo image_view_create_info{};
    image_view_create_info.sType            = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    image_view_create_info.format           = format;
    image_view_create_info.image            = image;
    image_view_create_info.viewType         = view_type;
    image_view_create_info.flags            = flags;
    image_view_create_info.subresourceRange = *subresource_range;
    image_view_create_info.pNext            = pNext;
    if (component_mapping == nullptr) {
        image_view_create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    } else {
        image_view_create_info.components = *component_mapping;
    }

    return image_view_create_info;
}

VkImageSubresourceLayers image_subresource_layers(VkImageAspectFlags aspect_flags, uint32_t mip_level, uint32_t base_layer, uint32_t layer_count) {
    VkImageSubresourceLayers subresource_layers{};
    subresource_layers.aspectMask     = aspect_flags;
    subresource_layers.mipLevel       = mip_level;
    subresource_layers.baseArrayLayer = base_layer;
    subresource_layers.layerCount     = layer_count;

    return subresource_layers;
}

VkImageBlit image_blit(VkImageSubresourceLayers src_subresource, VkImageSubresourceLayers dst_subresource, std::span<const VkOffset3D, 2> src_offsets,
                       std::span<const VkOffset3D, 2> dst_offsets) {
    VkImageBlit image_blit_region{};
    image_blit_region.srcSubresource = src_subresource;
    image_blit_region.srcOffsets[0]  = src_offsets[0];
    image_blit_region.srcOffsets[1]  = src_offsets[1];
    image_blit_region.dstSubresource = dst_subresource;
    image_blit_region.dstOffsets[0]  = dst_offsets[0];
    image_blit_region.dstOffsets[1]  = dst_offsets[1];

    return image_blit_region;
}

VkSamplerCreateInfo sampler_create_info(VkFilter mag_filter, VkFilter min_filter, VkSamplerAddressMode address_mode_u,
                                        VkSamplerAddressMode address_mode_v, VkSamplerAddressMode address_mode_w, bool anisotropy_enable,
                                        float max_anisotropy, VkSamplerMipmapMode mipmap_mode, float min_lod, float max_lod, float lod_bias,
                                        bool compare_enable, VkCompareOp compare_op, VkBorderColor border_color, bool unnormalized_coordinates,
                                        VkSamplerCreateFlags flags, const void* pNext) {
    VkSamplerCreateInfo sampler_create_info{};
    sampler_create_info.sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    sampler_create_info.magFilter               = mag_filter;
    sampler_create_info.minFilter               = min_filter;
    sampler_create_info.addressModeU            = address_mode_u;
    sampler_create_info.addressModeV            = address_mode_v;
    sampler_create_info.addressModeW            = address_mode_w;
    sampler_create_info.mipmapMode              = mipmap_mode;
    sampler_create_info.mipLodBias              = lod_bias;
    sampler_create_info.minLod                  = min_lod;
    sampler_create_info.maxLod                  = max_lod;
    sampler_create_info.compareEnable           = compare_enable;
    sampler_create_info.compareOp               = compare_op;
    sampler_create_info.anisotropyEnable        = anisotropy_enable;
    sampler_create_info.maxAnisotropy           = max_anisotropy;
    sampler_create_info.borderColor             = border_color;
    sampler_create_info.unnormalizedCoordinates = unnormalized_coordinates;
    sampler_create_info.flags                   = flags;
    sampler_create_info.pNext                   = pNext;

    return sampler_create_info;
}

VkBufferCreateInfo buffer_create_info(VkBufferUsageFlags usage, uint64_t size, VkBufferCreateFlags flags, VkSharingMode sharing_mode,
                                      std::span<const uint32_t> queue_family_indices, const void* pNext) {
    VkBufferCreateInfo buffer_create_info{};
    buffer_create_info.sType                 = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_create_info.usage                 = usage;
    buffer_create_info.size                  = size;
    buffer_create_info.flags                 = flags;
    buffer_create_info.sharingMode           = sharing_mode;
    buffer_create_info.queueFamilyIndexCount = queue_family_indices.size();
    buffer_create_info.pQueueFamilyIndices   = queue_family_indices.data();
    buffer_create_info.pNext                 = pNext;

    return buffer_create_info;
}

VkBufferViewCreateInfo buffer_view_create_info(VkFormat format, VkBuffer buffer, VkDeviceSize offset, VkDeviceSize range, const void* pNext) {
    VkBufferViewCreateInfo buffer_view_create_info{};
    buffer_view_create_info.sType  = VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO;
    buffer_view_create_info.buffer = buffer;
    buffer_view_create_info.format = format;
    buffer_view_create_info.offset = offset;
    buffer_view_create_info.range  = range;
    buffer_view_create_info.pNext  = pNext;

    return buffer_view_create_info;
}

VkBufferImageCopy buffer_image_copy(VkImageSubresourceLayers image_subresource, VkExtent3D image_extent, uint64_t buffer_offset,
                                    VkOffset3D image_offset, uint32_t buffer_row_length, uint32_t buffer_image_height) {
    VkBufferImageCopy buffer_image_copy{};
    buffer_image_copy.imageSubresource  = image_subresource;
    buffer_image_copy.imageExtent       = image_extent;
    buffer_image_copy.bufferOffset      = buffer_offset;
    buffer_image_copy.imageOffset       = image_offset;
    buffer_image_copy.bufferRowLength   = buffer_row_length;
    buffer_image_copy.bufferImageHeight = buffer_image_height;

    return buffer_image_copy;
}

VkBufferCopy buffer_copy(uint64_t size, uint64_t src_offset, uint64_t dst_offset) {
    VkBufferCopy buffer_copy{};
    buffer_copy.srcOffset = src_offset;
    buffer_copy.dstOffset = dst_offset;
    buffer_copy.size      = size;

    return buffer_copy;
}

VkImageCopy image_copy(VkImageSubresourceLayers src_subresource, VkImageSubresourceLayers dst_subresource, VkExtent3D extent, VkOffset3D src_offset,
                       VkOffset3D dst_offset) {
    VkImageCopy image_copy{};
    image_copy.srcSubresource = src_subresource;
    image_copy.dstSubresource = dst_subresource;
    image_copy.extent         = extent;
    image_copy.srcOffset      = src_offset;
    image_copy.dstOffset      = dst_offset;

    return image_copy;
}

} // namespace vk_lib