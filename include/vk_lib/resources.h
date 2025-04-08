/*
 * Utilities regarding memory management, images, buffers, and acceleration structures
 */

#pragma once
#include <vk_lib/common.h>

namespace vk_lib {

[[nodiscard]] VkImageCreateInfo
image_create_info(VkFormat format, VkImageUsageFlags usage, VkExtent3D extent, VkImageLayout initial_layout = VK_IMAGE_LAYOUT_UNDEFINED,
                  uint32_t mip_levels = 1, uint32_t array_layers = 1, VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT,
                  VkImageType type = VK_IMAGE_TYPE_2D, VkImageCreateFlags flags = 0, VkSharingMode sharing_mode = VK_SHARING_MODE_EXCLUSIVE,
                  std::span<const uint32_t> queue_family_indices = {}, VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL, const void* pNext = nullptr);

[[nodiscard]] VkImageSubresourceRange image_subresource_range(VkImageAspectFlags aspect_flags, uint32_t mip_level_count = 1,
                                                              uint32_t base_mip_level = 0, uint32_t array_layer_count = 1,
                                                              uint32_t base_array_layer = 0);

// subresource range MUST NOT be null
[[nodiscard]] VkImageViewCreateInfo image_view_create_info(VkFormat format, VkImage image, const VkImageSubresourceRange* subresource_range,
                                                           VkImageViewType view_type = VK_IMAGE_VIEW_TYPE_2D, VkImageCreateFlags flags = 0,
                                                           const VkComponentMapping* component_mapping = nullptr, const void* pNext = nullptr);

[[nodiscard]] VkImageSubresourceLayers image_subresource_layers(VkImageAspectFlags aspect_flags, uint32_t mip_level = 0, uint32_t base_layer = 0,
                                                                uint32_t layer_count = 1);

[[nodiscard]] VkImageBlit image_blit(VkImageSubresourceLayers src_subresource, VkImageSubresourceLayers dst_subresource,
                                     std::span<const VkOffset3D, 2> src_offsets, std::span<const VkOffset3D, 2> dst_offsets);

[[nodiscard]] VkSamplerCreateInfo sampler_create_info(
    VkFilter mag_filter = VK_FILTER_LINEAR, VkFilter min_filter = VK_FILTER_LINEAR,
    VkSamplerAddressMode address_mode_u = VK_SAMPLER_ADDRESS_MODE_REPEAT, VkSamplerAddressMode address_mode_v = VK_SAMPLER_ADDRESS_MODE_REPEAT,
    VkSamplerAddressMode address_mode_w = VK_SAMPLER_ADDRESS_MODE_REPEAT, bool anisotropy_enable = true, float max_anisotropy = 1.f,
    VkSamplerMipmapMode mipmap_mode = VK_SAMPLER_MIPMAP_MODE_LINEAR, float min_lod = 0.f, float max_lod = VK_LOD_CLAMP_NONE, float lod_bias = 0.f,
    bool compare_enable = false, VkCompareOp compare_op = VK_COMPARE_OP_ALWAYS, VkBorderColor border_color = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
    bool unnormalized_coordinates = false, VkSamplerCreateFlags flags = 0, const void* pNext = nullptr);

[[nodiscard]] VkBufferCreateInfo buffer_create_info(VkBufferUsageFlags usage, uint64_t size, VkBufferUsageFlags flags = 0,
                                                    VkSharingMode             sharing_mode         = VK_SHARING_MODE_EXCLUSIVE,
                                                    std::span<const uint32_t> queue_family_indices = {}, const void* pNext = nullptr);

[[nodiscard]] VkBufferViewCreateInfo buffer_view_create_info(VkFormat format, VkBuffer buffer, uint64_t offset = 0, uint64_t range = VK_WHOLE_SIZE,
                                                             const void* pNext = nullptr);

} // namespace vk_lib