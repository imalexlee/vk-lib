/*
 * Utilities regarding memory management, images, buffers, and acceleration structures
 */

#pragma once
#include <vk_lib/common.h>

namespace vk_lib {
struct ImageBuilder {
    VkImageCreateInfo     image_create_info{VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
    std::vector<uint32_t> queue_family_indices;
};

void image_builder_set_dimensions(ImageBuilder* builder, VkFormat format, uint32_t width, uint32_t height, uint32_t mip_levels = 1,
                                  uint32_t samples = 1, uint32_t array_layers = 1);

void image_builder_set_usage(ImageBuilder* builder, VkImageType image_type, VkImageUsageFlags usage_flags, VkImageAspectFlags aspect_flags,
                             VkImageCreateFlags image_create_flags = 0, VkImageLayout initial_layout = VK_IMAGE_LAYOUT_UNDEFINED,
                             VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL);

void image_builder_set_sharing_mode(ImageBuilder* builder, VkSharingMode sharing_mode, std::span<uint32_t> queue_family_indices = {});

void image_builder_set_pNext(ImageBuilder* builder, const void* pNext);

void image_builder_clear(ImageBuilder* builder);

VkResult image_builder_allocated_image_create(const ImageBuilder* builder, VkDevice device, VkImage* image, VmaAllocator allocator,
                                              VmaAllocation* allocation, VmaAllocationInfo* allocation_info = nullptr,
                                              VmaMemoryUsage memory_usage = VMA_MEMORY_USAGE_AUTO, VmaAllocationCreateFlags allocation_flags = 0);

void allocated_image_destroy(VkImage image, VmaAllocator allocator, VmaAllocation allocation);

VkResult image_builder_image_create(const ImageBuilder* builder, VkDevice device, VkImage* image);

void image_destroy(VkDevice device, VkImage image);

VkResult image_view_create(VkDevice device, VkImage image, VkImageViewType view_type, VkFormat format, VkImageAspectFlags aspect_flags,
                           VkImageView* image_view, const VkComponentMapping* component_mapping = nullptr, uint32_t mip_levels = 1,
                           uint32_t array_layers = 1, const void* pNext = nullptr);

void image_view_destroy(VkDevice device, VkImageView image_view);

[[nodiscard]] VkImageSubresourceRange image_subresource_range_create(VkImageAspectFlags aspect_flags, uint32_t base_array_layer,
                                                                     uint32_t array_layer_count, uint32_t base_mip_level, uint32_t mip_level_count);

[[nodiscard]] VkSamplerCreateInfo sampler_create_info(
    VkFilter mag_filter = VK_FILTER_LINEAR, VkFilter min_filter = VK_FILTER_LINEAR,
    VkSamplerAddressMode address_mode_u = VK_SAMPLER_ADDRESS_MODE_REPEAT, VkSamplerAddressMode address_mode_v = VK_SAMPLER_ADDRESS_MODE_REPEAT,
    VkSamplerAddressMode address_mode_w = VK_SAMPLER_ADDRESS_MODE_REPEAT, bool anisotropy_enable = true, float max_anisotropy = 1.f,
    VkSamplerMipmapMode mipmap_mode = VK_SAMPLER_MIPMAP_MODE_LINEAR, float min_lod = 0.f, float max_lod = VK_LOD_CLAMP_NONE, float lod_bias = 0.f,
    bool compare_enable = false, VkCompareOp compare_op = VK_COMPARE_OP_ALWAYS, VkBorderColor border_color = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
    bool unnormalized_coordinates = false, VkSamplerCreateFlags flags = 0, const void* pNext = nullptr);

[[nodiscard]] VkImageBlit image_blit_region_create(VkImageSubresourceLayers src_subresource, VkImageSubresourceLayers dst_subresource,
                                                   std::span<VkOffset3D, 2> src_offsets, std::span<VkOffset3D, 2> dst_offsets);

[[nodiscard]] VkImageSubresourceLayers image_subresource_layers_create(VkImageAspectFlags aspect_flags, uint32_t mip_level = 0,
                                                                       uint32_t base_layer = 0, uint32_t layer_count = 1);

// BEGIN BUFFER

struct BufferBuilder {
    VkBufferCreateInfo    buffer_create_info{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
    std::vector<uint32_t> queue_family_indices;
};

void buffer_builder_set_size_and_usage(BufferBuilder* builder, VkDeviceSize size, VkBufferUsageFlags usage_flags,
                                       VkBufferCreateFlags create_flags = 0);

void buffer_builder_set_sharing_mode(BufferBuilder* builder, VkSharingMode sharing_mode, std::span<uint32_t> queue_family_indices = {});

void buffer_builder_set_pNext(BufferBuilder* builder, const void* pNext);

void buffer_builder_clear(BufferBuilder* builder);

VkResult buffer_builder_allocated_buffer_create(const BufferBuilder* builder, VkBuffer* buffer, VmaAllocator allocator, VmaAllocation* allocation,
                                                VmaAllocationInfo* allocation_info = nullptr, VmaMemoryUsage memory_usage = VMA_MEMORY_USAGE_AUTO,
                                                VmaAllocationCreateFlags allocation_flags = 0);

void allocated_buffer_destroy(VkBuffer buffer, VmaAllocator allocator, VmaAllocation allocation);

VkResult buffer_builder_buffer_create(const BufferBuilder* builder, VkDevice device, VkBuffer* buffer);

void buffer_destroy(VkDevice device, VkBuffer buffer);

VkResult buffer_view_create(VkDevice device, VkBuffer buffer, VkFormat format, VkBufferView* buffer_view, VkDeviceSize offset = 0,
                            VkDeviceSize range = VK_WHOLE_SIZE, const void* pNext = nullptr);

[[nodiscard]] VkDeviceAddress buffer_device_address_get(VkDevice device, VkBuffer buffer);

void buffer_view_destroy(VkDevice device, VkBufferView buffer_view);

} // namespace vk_lib