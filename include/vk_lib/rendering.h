/*
 * Utilities regarding rendering
 */

#pragma once
#include <vk_lib/common.h>

VkRect2D rect_2d_create(uint32_t width, uint32_t height, int32_t x_offset = 0, int32_t y_offset = 0);

/*
 * CORE EXTENSIONS
 */

// VULKAN 1.3

[[nodiscard]] VkRenderingAttachmentInfoKHR
rendering_attachment_info_create(VkImageView image_view, VkImageLayout image_layout, VkAttachmentLoadOp load_op, VkAttachmentStoreOp store_op,
                                 const VkClearValue* clear_value = nullptr, VkResolveModeFlagBitsKHR resolve_mode = VK_RESOLVE_MODE_NONE_KHR,
                                 VkImageView resolve_image_view = nullptr, VkImageLayout resolve_image_layout = VK_IMAGE_LAYOUT_UNDEFINED);

[[nodiscard]] VkRenderingInfoKHR rendering_info_create(VkRect2D render_area, std::span<VkRenderingAttachmentInfoKHR> color_attachments,
                                                       const VkRenderingAttachmentInfoKHR* depth_attachment   = nullptr,
                                                       const VkRenderingAttachmentInfoKHR* stencil_attachment = nullptr,
                                                       VkRenderingFlagsKHR flags = 0, uint32_t view_mask = 0, uint32_t layer_count = 1,
                                                       const void* pNext = nullptr);

void rendering_begin(VkCommandBuffer command_buffer, const VkRenderingInfoKHR* rendering_info);

void rendering_end(VkCommandBuffer command_buffer);
