#include <vk_lib/resources.h>


void image_builder_set_dimensions(ImageBuilder* builder, VkFormat format, uint32_t width,
                                  uint32_t height, uint32_t depth, uint32_t mip_levels, uint32_t samples, uint32_t array_layers) {
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

void image_builder_set_usage(ImageBuilder* builder, VkImageType image_type, VkImageUsageFlags usage_flags,
                             VkImageCreateFlags image_create_flags,
                             VkImageLayout initial_layout,
                             VkImageTiling tiling
    ) {
    builder->image_create_info.imageType     = image_type;
    builder->image_create_info.usage         = usage_flags;
    builder->image_create_info.flags         = image_create_flags;
    builder->image_create_info.initialLayout = initial_layout;
    builder->image_create_info.tiling        = tiling;
}

void image_builder_set_sharing_mode(ImageBuilder* builder, VkSharingMode sharing_mode,
                                    std::span<uint32_t> queue_family_indices) {
    builder->queue_family_indices.clear();
    builder->queue_family_indices.reserve(queue_family_indices.size());
    for (uint32_t family_index : queue_family_indices) {
        builder->queue_family_indices.push_back(family_index);
    }
    builder->image_create_info.sharingMode           = sharing_mode;
    builder->image_create_info.pQueueFamilyIndices   = builder->queue_family_indices.data();
    builder->image_create_info.queueFamilyIndexCount = builder->queue_family_indices.size();
}

void image_builder_set_pNext(ImageBuilder* builder, const void* pNext) {
    builder->image_create_info.pNext = pNext;
}

void image_builder_clear(ImageBuilder* builder) {
    *builder = ImageBuilder{};
}

VkResult image_builder_allocated_image_create(const ImageBuilder* builder, VkDevice device,
                                              VkImage* image,
                                              VmaAllocator allocator,
                                              VmaAllocation* allocation,
                                              VmaAllocationInfo* allocation_info,
                                              VmaMemoryUsage memory_usage,
                                              VmaAllocationCreateFlags allocation_flags) {
    VmaAllocationCreateInfo allocation_create_info{};
    allocation_create_info.usage = memory_usage;
    allocation_create_info.flags = allocation_flags;

    return vmaCreateImage(allocator, &builder->image_create_info, &allocation_create_info, image, allocation, allocation_info);
}

void allocated_image_destroy(VkImage image, VmaAllocator allocator, VmaAllocation allocation) {
    vmaDestroyImage(allocator, image, allocation);
}

VkResult image_builder_unallocated_image_create(const ImageBuilder* builder, VkDevice device,
                                                VkImage* image) {
    return vkCreateImage(device, &builder->image_create_info, nullptr, image);
}

void unallocated_image_destroy(VkDevice device, VkImage image) {
    vkDestroyImage(device, image, nullptr);
}

VkResult image_view_create(VkDevice device, VkImage image,
                           VkImageViewType view_type,
                           VkFormat format,
                           VkImageAspectFlags aspect_flags,
                           VkImageView* image_view,
                           uint32_t mip_levels,
                           uint32_t array_layers,
                           const void* pNext
    ) {
    const VkImageSubresourceRange subresource_range = image_subresource_range_create(aspect_flags, 0, array_layers, 1, mip_levels);

    VkImageViewCreateInfo image_view_create_info{};
    image_view_create_info.sType            = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    image_view_create_info.pNext            = pNext;
    image_view_create_info.format           = format;
    image_view_create_info.image            = image;
    image_view_create_info.viewType         = view_type;
    image_view_create_info.subresourceRange = subresource_range;

    return vkCreateImageView(device, &image_view_create_info, nullptr, image_view);
}

void image_view_destroy(VkDevice device, VkImageView image_view) {
    vkDestroyImageView(device, image_view, nullptr);
}

VkImageSubresourceRange image_subresource_range_create(
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

void sampler_builder_set_filtering(SamplerBuilder* builder, VkFilter min_filter, VkFilter mag_filter, bool unnormalized_coordinates_enabled, VkBorderColor border_color) {
    builder->sampler_create_info.minFilter               = min_filter;
    builder->sampler_create_info.magFilter               = mag_filter;
    builder->sampler_create_info.borderColor             = border_color;
    builder->sampler_create_info.unnormalizedCoordinates = unnormalized_coordinates_enabled;
}

void sampler_builder_set_address_modes(SamplerBuilder* builder, VkSamplerAddressMode address_mode_u, VkSamplerAddressMode address_mode_v, VkSamplerAddressMode address_mode_w) {
    builder->sampler_create_info.addressModeU = address_mode_u;
    builder->sampler_create_info.addressModeV = address_mode_v;
    builder->sampler_create_info.addressModeW = address_mode_w;
}

void sampler_builder_set_lod(SamplerBuilder* builder, VkSamplerMipmapMode mipmap_mode, float min_lod, float max_lod, float mip_lod_bias) {
    builder->sampler_create_info.minLod     = min_lod;
    builder->sampler_create_info.maxLod     = max_lod;
    builder->sampler_create_info.mipLodBias = mip_lod_bias;
    builder->sampler_create_info.mipmapMode = mipmap_mode;
}

void sampler_builder_set_comparison(SamplerBuilder* builder, bool compare_enabled, VkCompareOp compare_operation) {
    builder->sampler_create_info.compareEnable = compare_enabled;
    builder->sampler_create_info.compareOp     = compare_operation;
}

void sampler_builder_set_anisotropy(SamplerBuilder* builder, bool anisotropy_enabled, float max_anisotropy) {
    builder->sampler_create_info.anisotropyEnable = anisotropy_enabled;
    builder->sampler_create_info.maxAnisotropy    = max_anisotropy;
}

void sampler_builder_set_pNext(SamplerBuilder* builder, const void* pNext) {
    builder->sampler_create_info.pNext = pNext;
}

void sampler_builder_clear(SamplerBuilder* builder) {
    *builder = SamplerBuilder{};
}

VkResult sampler_builder_sampler_create(const SamplerBuilder* builder, VkDevice device, VkSampler* sampler) {
    return vkCreateSampler(device, &builder->sampler_create_info, nullptr, sampler);
}

void sampler_destroy(VkDevice device, VkSampler sampler) {
    vkDestroySampler(device, sampler, nullptr);
}

void image_multi_blit(VkCommandBuffer command_buffer, VkImage src_image, VkImage dst_image, std::span<VkImageBlit> blit_regions, VkFilter filter, VkImageLayout src_layout, VkImageLayout dst_layout) {
    vkCmdBlitImage(command_buffer, src_image, src_layout, dst_image, dst_layout, blit_regions.size(), blit_regions.data(), filter);
}

void image_blit(VkCommandBuffer command_buffer, VkImage src_image, VkImage dst_image, const VkImageBlit* blit_region, VkFilter filter, VkImageLayout src_layout, VkImageLayout dst_layout) {
    vkCmdBlitImage(command_buffer, src_image, src_layout, dst_image, dst_layout, 1, blit_region, filter);
}

VkImageBlit image_blit_region_create(VkImageSubresourceLayers src_subresource, VkImageSubresourceLayers dst_subresource, std::span<VkOffset3D, 2> src_offsets, std::span<VkOffset3D, 2> dst_offsets) {
    VkImageBlit image_blit_region{};
    image_blit_region.srcSubresource = src_subresource;
    image_blit_region.srcOffsets[0]  = src_offsets[0];
    image_blit_region.srcOffsets[1]  = src_offsets[1];
    image_blit_region.dstSubresource = dst_subresource;
    image_blit_region.dstOffsets[0]  = dst_offsets[0];
    image_blit_region.dstOffsets[1]  = dst_offsets[1];

    return image_blit_region;
}

VkImageSubresourceLayers image_subresource_layers_create(VkImageAspectFlags aspect_flags, uint32_t mip_level, uint32_t base_layer, uint32_t layer_count) {
    VkImageSubresourceLayers subresource_layers{};
    subresource_layers.aspectMask     = aspect_flags;
    subresource_layers.mipLevel       = mip_level;
    subresource_layers.baseArrayLayer = base_layer;
    subresource_layers.layerCount     = layer_count;

    return subresource_layers;
}

void buffer_builder_set_size_and_usage(BufferBuilder* builder,
                                       VkDeviceSize size,
                                       VkBufferUsageFlags usage_flags,
                                       VkBufferCreateFlags create_flags) {
    builder->buffer_create_info.size  = size;
    builder->buffer_create_info.usage = usage_flags;
    builder->buffer_create_info.flags = create_flags;
}

void buffer_builder_set_sharing_mode(BufferBuilder* builder, VkSharingMode sharing_mode,
                                     std::span<uint32_t> queue_family_indices) {
    builder->queue_family_indices.clear();
    builder->queue_family_indices.reserve(queue_family_indices.size());
    for (uint32_t family_index : queue_family_indices) {
        builder->queue_family_indices.push_back(family_index);
    }
    builder->buffer_create_info.sharingMode           = sharing_mode;
    builder->buffer_create_info.pQueueFamilyIndices   = builder->queue_family_indices.data();
    builder->buffer_create_info.queueFamilyIndexCount = builder->queue_family_indices.size();
}

void buffer_builder_set_pNext(BufferBuilder* builder, const void* pNext) {
    builder->buffer_create_info.pNext = pNext;
}

void buffer_builder_clear(BufferBuilder* builder) {
    *builder = BufferBuilder{};
}

VkResult buffer_builder_allocated_buffer_create(const BufferBuilder* builder,
                                                VkBuffer* buffer,
                                                VmaAllocator allocator,
                                                VmaAllocation* allocation,
                                                VmaAllocationInfo* allocation_info,
                                                VmaMemoryUsage memory_usage,
                                                VmaAllocationCreateFlags allocation_flags) {

    VmaAllocationCreateInfo allocation_create_info{};
    allocation_create_info.usage = memory_usage;
    allocation_create_info.flags = allocation_flags;

    return vmaCreateBuffer(allocator, &builder->buffer_create_info, &allocation_create_info, buffer, allocation, allocation_info);
}

void allocated_buffer_destroy(VkBuffer buffer, VmaAllocator allocator, VmaAllocation allocation) {
    vmaDestroyBuffer(allocator, buffer, allocation);
}

VkResult buffer_builder_unallocated_buffer_create(const BufferBuilder* builder, VkDevice device,
                                                  VkBuffer* buffer) {
    return vkCreateBuffer(device, &builder->buffer_create_info, nullptr, buffer);
}

void unallocated_buffer_destroy(VkDevice device, VkBuffer buffer) {
    vkDestroyBuffer(device, buffer, nullptr);
}

VkResult buffer_view_create(VkDevice device, VkBuffer buffer, VkFormat format, VkBufferView* buffer_view, VkDeviceSize offset, VkDeviceSize range, const void* pNext) {
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

void buffer_view_destroy(VkDevice device, VkBufferView buffer_view) {
    vkDestroyBufferView(device, buffer_view, nullptr);
}

VkDeviceAddress buffer_device_address_get(VkDevice device, VkBuffer buffer) {
    VkBufferDeviceAddressInfo buffer_device_address_info{};
    buffer_device_address_info.sType  = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO;
    buffer_device_address_info.buffer = buffer;

    return vkGetBufferDeviceAddress(device, &buffer_device_address_info);
}



