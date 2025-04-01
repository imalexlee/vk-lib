#include <vk_lib/resources.h>

namespace vk_lib {
void image_builder_set_dimensions(ImageBuilder* builder, VkFormat format, uint32_t width, uint32_t height, uint32_t depth, uint32_t mip_levels,
                                  uint32_t samples, uint32_t array_layers) {
    VkExtent3D extent_3d{};
    extent_3d.width  = width;
    extent_3d.height = height;
    extent_3d.depth  = depth;

    builder->image_create_info.extent      = extent_3d;
    builder->image_create_info.format      = format;
    builder->image_create_info.mipLevels   = mip_levels;
    builder->image_create_info.samples     = static_cast<VkSampleCountFlagBits>(samples);
    builder->image_create_info.arrayLayers = array_layers;
}

void image_builder_set_usage(ImageBuilder* builder, VkImageType image_type, VkImageUsageFlags usage_flags, VkImageCreateFlags image_create_flags,
                             VkImageLayout initial_layout, VkImageTiling tiling) {
    builder->image_create_info.imageType     = image_type;
    builder->image_create_info.usage         = usage_flags;
    builder->image_create_info.flags         = image_create_flags;
    builder->image_create_info.initialLayout = initial_layout;
    builder->image_create_info.tiling        = tiling;
}

void image_builder_set_sharing_mode(ImageBuilder* builder, VkSharingMode sharing_mode, std::span<uint32_t> queue_family_indices) {
    builder->queue_family_indices.clear();
    builder->queue_family_indices.reserve(queue_family_indices.size());
    for (uint32_t family_index : queue_family_indices) {
        builder->queue_family_indices.push_back(family_index);
    }
    builder->image_create_info.sharingMode           = sharing_mode;
    builder->image_create_info.pQueueFamilyIndices   = builder->queue_family_indices.data();
    builder->image_create_info.queueFamilyIndexCount = builder->queue_family_indices.size();
}

void image_builder_set_pNext(ImageBuilder* builder, const void* pNext) { builder->image_create_info.pNext = pNext; }

void image_builder_clear(ImageBuilder* builder) { *builder = ImageBuilder{}; }

VkResult image_builder_allocated_image_create(const ImageBuilder* builder, VkDevice device, VkImage* image, VmaAllocator allocator,
                                              VmaAllocation* allocation, VmaAllocationInfo* allocation_info, VmaMemoryUsage memory_usage,
                                              VmaAllocationCreateFlags allocation_flags) {
    VmaAllocationCreateInfo allocation_create_info{};
    allocation_create_info.usage = memory_usage;
    allocation_create_info.flags = allocation_flags;

    return vmaCreateImage(allocator, &builder->image_create_info, &allocation_create_info, image, allocation, allocation_info);
}

void allocated_image_destroy(VkImage image, VmaAllocator allocator, VmaAllocation allocation) { vmaDestroyImage(allocator, image, allocation); }

VkResult image_builder_image_create(const ImageBuilder* builder, VkDevice device, VkImage* image) {
    return vkCreateImage(device, &builder->image_create_info, nullptr, image);
}

void image_destroy(VkDevice device, VkImage image) { vkDestroyImage(device, image, nullptr); }

VkResult image_view_create(VkDevice device, VkImage image, VkImageViewType view_type, VkFormat format, VkImageAspectFlags aspect_flags,
                           VkImageView* image_view, const VkComponentMapping* component_mapping, uint32_t mip_levels, uint32_t array_layers,
                           const void* pNext) {
    const VkImageSubresourceRange subresource_range = image_subresource_range_create(aspect_flags, 0, array_layers, 0, mip_levels);

    VkImageViewCreateInfo image_view_create_info{};
    image_view_create_info.sType            = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    image_view_create_info.pNext            = pNext;
    image_view_create_info.format           = format;
    image_view_create_info.image            = image;
    image_view_create_info.viewType         = view_type;
    image_view_create_info.subresourceRange = subresource_range;
    if (component_mapping == nullptr) {
        image_view_create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    } else {
        image_view_create_info.components = *component_mapping;
    }

    return vkCreateImageView(device, &image_view_create_info, nullptr, image_view);
}

void image_view_destroy(VkDevice device, VkImageView image_view) { vkDestroyImageView(device, image_view, nullptr); }

VkImageSubresourceRange image_subresource_range_create(VkImageAspectFlags aspect_flags, uint32_t base_array_layer, uint32_t array_layer_count,
                                                       uint32_t base_mip_level, uint32_t mip_level_count) {
    VkImageSubresourceRange subresource_range{};
    subresource_range.aspectMask     = aspect_flags;
    subresource_range.baseArrayLayer = base_array_layer;
    subresource_range.layerCount     = array_layer_count;
    subresource_range.baseMipLevel   = base_mip_level;
    subresource_range.levelCount     = mip_level_count;

    return subresource_range;
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

VkImageBlit image_blit_region_create(VkImageSubresourceLayers src_subresource, VkImageSubresourceLayers dst_subresource,
                                     std::span<VkOffset3D, 2> src_offsets, std::span<VkOffset3D, 2> dst_offsets) {
    VkImageBlit image_blit_region{};
    image_blit_region.srcSubresource = src_subresource;
    image_blit_region.srcOffsets[0]  = src_offsets[0];
    image_blit_region.srcOffsets[1]  = src_offsets[1];
    image_blit_region.dstSubresource = dst_subresource;
    image_blit_region.dstOffsets[0]  = dst_offsets[0];
    image_blit_region.dstOffsets[1]  = dst_offsets[1];

    return image_blit_region;
}

VkImageSubresourceLayers image_subresource_layers_create(VkImageAspectFlags aspect_flags, uint32_t mip_level, uint32_t base_layer,
                                                         uint32_t layer_count) {
    VkImageSubresourceLayers subresource_layers{};
    subresource_layers.aspectMask     = aspect_flags;
    subresource_layers.mipLevel       = mip_level;
    subresource_layers.baseArrayLayer = base_layer;
    subresource_layers.layerCount     = layer_count;

    return subresource_layers;
}

void buffer_builder_set_size_and_usage(BufferBuilder* builder, VkDeviceSize size, VkBufferUsageFlags usage_flags, VkBufferCreateFlags create_flags) {
    builder->buffer_create_info.size  = size;
    builder->buffer_create_info.usage = usage_flags;
    builder->buffer_create_info.flags = create_flags;
}

void buffer_builder_set_sharing_mode(BufferBuilder* builder, VkSharingMode sharing_mode, std::span<uint32_t> queue_family_indices) {
    builder->queue_family_indices.clear();
    builder->queue_family_indices.reserve(queue_family_indices.size());
    for (uint32_t family_index : queue_family_indices) {
        builder->queue_family_indices.push_back(family_index);
    }
    builder->buffer_create_info.sharingMode           = sharing_mode;
    builder->buffer_create_info.pQueueFamilyIndices   = builder->queue_family_indices.data();
    builder->buffer_create_info.queueFamilyIndexCount = builder->queue_family_indices.size();
}

void buffer_builder_set_pNext(BufferBuilder* builder, const void* pNext) { builder->buffer_create_info.pNext = pNext; }

void buffer_builder_clear(BufferBuilder* builder) { *builder = BufferBuilder{}; }

VkResult buffer_builder_allocated_buffer_create(const BufferBuilder* builder, VkBuffer* buffer, VmaAllocator allocator, VmaAllocation* allocation,
                                                VmaAllocationInfo* allocation_info, VmaMemoryUsage memory_usage,
                                                VmaAllocationCreateFlags allocation_flags) {
    VmaAllocationCreateInfo allocation_create_info{};
    allocation_create_info.usage = memory_usage;
    allocation_create_info.flags = allocation_flags;

    return vmaCreateBuffer(allocator, &builder->buffer_create_info, &allocation_create_info, buffer, allocation, allocation_info);
}

void allocated_buffer_destroy(VkBuffer buffer, VmaAllocator allocator, VmaAllocation allocation) { vmaDestroyBuffer(allocator, buffer, allocation); }

VkResult buffer_builder_buffer_create(const BufferBuilder* builder, VkDevice device, VkBuffer* buffer) {
    return vkCreateBuffer(device, &builder->buffer_create_info, nullptr, buffer);
}

void buffer_destroy(VkDevice device, VkBuffer buffer) { vkDestroyBuffer(device, buffer, nullptr); }

VkResult buffer_view_create(VkDevice device, VkBuffer buffer, VkFormat format, VkBufferView* buffer_view, VkDeviceSize offset, VkDeviceSize range,
                            const void* pNext) {
    VkBufferViewCreateInfo buffer_view_create_info{};
    buffer_view_create_info.sType  = VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO;
    buffer_view_create_info.buffer = buffer;
    buffer_view_create_info.format = format;
    buffer_view_create_info.offset = offset;
    buffer_view_create_info.range  = range;
    buffer_view_create_info.pNext  = pNext;
    buffer_view_create_info.flags  = 0; // vulkan specs states this parameter is unused currently

    return vkCreateBufferView(device, &buffer_view_create_info, nullptr, buffer_view);
}

void buffer_view_destroy(VkDevice device, VkBufferView buffer_view) { vkDestroyBufferView(device, buffer_view, nullptr); }

VkDeviceAddress buffer_device_address_get(VkDevice device, VkBuffer buffer) {
    VkBufferDeviceAddressInfo buffer_device_address_info{};
    buffer_device_address_info.sType  = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO_KHR;
    buffer_device_address_info.buffer = buffer;

    return vkGetBufferDeviceAddress(device, &buffer_device_address_info);
}

} // namespace vk_lib