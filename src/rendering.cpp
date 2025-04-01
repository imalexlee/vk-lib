#include <vk_lib/rendering.h>

namespace vk_lib {

VkRect2D rect_2d(uint32_t width, uint32_t height, int32_t x_offset, int32_t y_offset) {
    const VkOffset2D offset = {.x = x_offset, .y = y_offset};
    const VkExtent2D extent = {.width = width, .height = height};

    return VkRect2D{.offset = offset, .extent = extent};
}

VkViewport viewport(float width, float height, float x_offset, float y_offset, float min_depth, float max_depth) {
    VkViewport viewport{};
    viewport.width    = width;
    viewport.height   = height;
    viewport.x        = x_offset;
    viewport.y        = y_offset;
    viewport.minDepth = min_depth;
    viewport.maxDepth = max_depth;
    return viewport;
}

VkRenderingAttachmentInfoKHR rendering_attachment_info(VkImageView image_view, VkImageLayout image_layout, VkAttachmentLoadOp load_op,
                                                       VkAttachmentStoreOp store_op, const VkClearValue* clear_value,
                                                       VkResolveModeFlagBitsKHR resolve_mode, VkImageView resolve_image_view,
                                                       VkImageLayout resolve_image_layout) {
    VkRenderingAttachmentInfoKHR rendering_attachment_info{};
    rendering_attachment_info.sType              = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO_KHR;
    rendering_attachment_info.imageView          = image_view;
    rendering_attachment_info.imageLayout        = image_layout;
    rendering_attachment_info.loadOp             = load_op;
    rendering_attachment_info.storeOp            = store_op;
    rendering_attachment_info.resolveMode        = resolve_mode;
    rendering_attachment_info.resolveImageView   = resolve_image_view;
    rendering_attachment_info.resolveImageLayout = resolve_image_layout;
    if (clear_value != nullptr) {
        rendering_attachment_info.clearValue = *clear_value;
    }

    return rendering_attachment_info;
}

VkRenderingInfoKHR rendering_info(VkRect2D render_area, std::span<VkRenderingAttachmentInfoKHR> color_attachments,
                                  const VkRenderingAttachmentInfoKHR* depth_attachment, const VkRenderingAttachmentInfoKHR* stencil_attachment,
                                  VkRenderingFlagsKHR flags, uint32_t view_mask, uint32_t layer_count, const void* pNext) {
    VkRenderingInfoKHR rendering_info{};
    rendering_info.sType                = VK_STRUCTURE_TYPE_RENDERING_INFO_KHR;
    rendering_info.flags                = flags;
    rendering_info.renderArea           = render_area;
    rendering_info.layerCount           = layer_count;
    rendering_info.viewMask             = view_mask;
    rendering_info.colorAttachmentCount = color_attachments.size();
    rendering_info.pColorAttachments    = color_attachments.data();
    rendering_info.pDepthAttachment     = depth_attachment;
    rendering_info.pStencilAttachment   = stencil_attachment;
    rendering_info.pNext                = pNext;

    return rendering_info;
}

} // namespace vk_lib