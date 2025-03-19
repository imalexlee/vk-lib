/*
* Utilities regarding memory management, images, buffers, and acceleration structures
*/

#pragma once
#include <vk_lib/common.h>
#include <vk_mem_alloc.h>

struct ImageBuilder
{
    VkImageCreateInfo image_create_info{VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
    std::vector<uint32_t> queue_family_indices;
};

void image_builder_set_dimensions(ImageBuilder* builder, VkFormat format, uint32_t width,
                                  uint32_t height, uint32_t mip_levels = 1, uint32_t samples = 1,
                                  uint32_t array_layers = 1);

void image_builder_set_usage(ImageBuilder* builder,
                             VkImageType image_type,
                             VkImageUsageFlags usage_flags,
                             VkImageAspectFlags aspect_flags,
                             VkImageCreateFlags image_create_flags = 0,
                             VkImageLayout initial_layout = VK_IMAGE_LAYOUT_UNDEFINED,
                             VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL
);

void image_builder_set_sharing_mode(ImageBuilder* builder, VkSharingMode sharing_mode,
                                    std::span<uint32_t> queue_family_indices = {});

void image_builder_set_pNext_chain(ImageBuilder* builder, const void* chain);

VkResult image_builder_allocated_image_create(const ImageBuilder* builder, VkDevice device,
                                              VkImage* image,
                                              VmaAllocator allocator,
                                              VmaAllocation* allocation,
                                              VmaAllocationInfo* allocation_info = nullptr,
                                              VmaMemoryUsage memory_usage = VMA_MEMORY_USAGE_AUTO,
                                              VmaAllocationCreateFlags allocation_flags = 0);

VkResult image_builder_unallocated_image_create(const ImageBuilder* builder, VkDevice device,
                                                VkImage* image);

VkResult image_view_create(VkDevice device, VkImage image,
                           VkImageViewType view_type,
                           VkFormat format,
                           VkImageAspectFlags aspect_flags,
                           VkImageView* image_view,
                           uint32_t mip_levels = 1,
                           uint32_t array_layers = 1);

void allocated_buffer_destroy(VkImage image, VmaAllocator allocator, VmaAllocation allocation);

void unallocated_image_destroy(VkDevice device, VkImage image);

void image_view_destroy(VkDevice device, VkImageView image_view);


[[nodiscard]] VkImageSubresourceRange image_subresource_range_create(
    VkImageAspectFlags aspect_flags,
    uint32_t base_array_layer,
    uint32_t array_layer_count,
    uint32_t base_mip_level,
    uint32_t mip_level_count);

struct SamplerBuilder
{
    VkSamplerCreateInfo sampler_create_info{VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO};
};

void sampler_builder_set_filtering(SamplerBuilder* builder, VkFilter min_filter,
                                   VkFilter mag_filter,
                                   bool unnormalized_coordinates_enabled = false,
                                   VkBorderColor border_color =
                                       VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK);

void sampler_builder_set_address_modes(SamplerBuilder* builder,
                                       VkSamplerAddressMode address_mode_u,
                                       VkSamplerAddressMode address_mode_v,
                                       VkSamplerAddressMode address_mode_w);

void sampler_builder_set_lod(SamplerBuilder* builder, VkSamplerMipmapMode mipmap_mode,
                             float min_lod, float max_lod, float mip_lod_bias);

void sampler_builder_set_comparison(SamplerBuilder* builder, bool compare_enabled,
                                    VkCompareOp compare_operation);

void sampler_builder_set_anisotropy(SamplerBuilder* builder, bool anisotropy_enabled,
                                    float max_anisotropy);

VkResult sampler_builder_sampler_create(const SamplerBuilder* builder, VkDevice device,
                                        VkSampler* sampler);

void sampler_destroy(VkDevice device, VkSampler sampler);

void image_multi_blit(VkCommandBuffer command_buffer, VkImage src_image, VkImage dst_image,
                      std::span<VkImageBlit> blit_regions, VkFilter filter = VK_FILTER_NEAREST,
                      VkImageLayout src_layout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                      VkImageLayout dst_layout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

void image_blit(VkCommandBuffer command_buffer, VkImage src_image, VkImage dst_image,
                const VkImageBlit* blit_region, VkFilter filter = VK_FILTER_NEAREST,
                VkImageLayout src_layout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                VkImageLayout dst_layout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

[[nodiscard]] VkImageBlit image_blit_region_create(VkImageSubresourceLayers src_subresource,
                                                   VkImageSubresourceLayers dst_subresource,
                                                   std::span<VkOffset3D, 2> src_offsets,
                                                   std::span<VkOffset3D, 2> dst_offsets);

[[nodiscard]] VkImageSubresourceLayers image_subresource_layers_create(
    VkImageAspectFlags aspect_flags,
    uint32_t mip_level = 0,
    uint32_t base_layer = 0,
    uint32_t layer_count = 1);

// BEGIN BUFFER

struct BufferBuilder
{
    VkBufferCreateInfo buffer_create_info{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
    std::vector<uint32_t> queue_family_indices;
};

void buffer_builder_set_size_and_usage(BufferBuilder* builder,
                                       VkDeviceSize size,
                                       VkBufferUsageFlags usage_flags,
                                       VkBufferCreateFlags create_flags = 0);

void buffer_builder_set_sharing_mode(BufferBuilder* builder, VkSharingMode sharing_mode,
                                     std::span<uint32_t> queue_family_indices = {});

void buffer_builder_set_pNext_chain(BufferBuilder* builder, const void* chain);

VkResult buffer_builder_allocated_buffer_create(const BufferBuilder* builder,
                                                VkBuffer* buffer,
                                                VmaAllocator allocator,
                                                VmaAllocation* allocation,
                                                VmaAllocationInfo* allocation_info = nullptr,
                                                VmaMemoryUsage memory_usage = VMA_MEMORY_USAGE_AUTO,
                                                VmaAllocationCreateFlags allocation_flags = 0);

VkResult buffer_builder_unallocated_buffer_create(const BufferBuilder* builder, VkDevice device,
                                                  VkBuffer* buffer);

VkResult buffer_view_create(VkDevice device, VkBuffer buffer, VkFormat format,
                            VkBufferView* buffer_view, VkDeviceSize offset = 0,
                            VkDeviceSize range = VK_WHOLE_SIZE,
                            void* pNext_chain = nullptr);

[[nodiscard]] VkDeviceAddress buffer_device_address_get(VkDevice device, VkBuffer buffer);

void allocated_buffer_destroy(VkBuffer buffer, VmaAllocator allocator, VmaAllocation allocation);

void unallocated_buffer_destroy(VkDevice device, VkBuffer buffer);

void buffer_view_destroy(VkDevice device, VkBufferView buffer_view);
