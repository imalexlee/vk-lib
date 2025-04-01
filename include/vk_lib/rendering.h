/*
 * Utilities regarding rendering
 */

#pragma once
#include <vk_lib/common.h>

namespace vk_lib {

[[nodiscard]] VkRect2D rect_2d(uint32_t width, uint32_t height, int32_t x_offset = 0, int32_t y_offset = 0);

[[nodiscard]] VkViewport viewport(float width, float height, float x_offset = 0, float y_offset = 0, float min_depth = 0, float max_depth = 1);

// TODO: add render pass initializers
/*
 * CORE EXTENSIONS
 */

// VULKAN 1.3

[[nodiscard]] VkRenderingAttachmentInfoKHR rendering_attachment_info(VkImageView image_view, VkImageLayout image_layout, VkAttachmentLoadOp load_op,
                                                                     VkAttachmentStoreOp store_op, const VkClearValue* clear_value = nullptr,
                                                                     VkResolveModeFlagBitsKHR resolve_mode         = VK_RESOLVE_MODE_NONE_KHR,
                                                                     VkImageView              resolve_image_view   = nullptr,
                                                                     VkImageLayout            resolve_image_layout = VK_IMAGE_LAYOUT_UNDEFINED);

[[nodiscard]] VkRenderingInfoKHR rendering_info(VkRect2D render_area, std::span<VkRenderingAttachmentInfoKHR> color_attachments,
                                                const VkRenderingAttachmentInfoKHR* depth_attachment   = nullptr,
                                                const VkRenderingAttachmentInfoKHR* stencil_attachment = nullptr, VkRenderingFlagsKHR flags = 0,
                                                uint32_t view_mask = 0, uint32_t layer_count = 1, const void* pNext = nullptr);
} // namespace vk_lib