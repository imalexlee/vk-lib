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

VkAttachmentDescription attachment_description(VkFormat format, VkImageLayout initial_layout, VkImageLayout final_layout, VkAttachmentLoadOp load_op,
                                               VkAttachmentStoreOp store_op, VkAttachmentLoadOp stencil_load_op, VkAttachmentStoreOp stencil_store_op,
                                               VkSampleCountFlagBits samples, VkAttachmentDescriptionFlags flags) {
    VkAttachmentDescription attachment_description{};
    attachment_description.format         = format;
    attachment_description.initialLayout  = initial_layout;
    attachment_description.finalLayout    = final_layout;
    attachment_description.loadOp         = load_op;
    attachment_description.storeOp        = store_op;
    attachment_description.stencilLoadOp  = stencil_load_op;
    attachment_description.stencilStoreOp = stencil_store_op;
    attachment_description.samples        = samples;
    attachment_description.flags          = flags;

    return attachment_description;
}

VkAttachmentReference attachment_reference(uint32_t attachment, VkImageLayout layout) {
    VkAttachmentReference attachment_reference{};
    attachment_reference.attachment = attachment;
    attachment_reference.layout     = layout;

    return attachment_reference;
}

VkSubpassDescription subpass_description(std::span<VkAttachmentReference> color_attachments, const VkAttachmentReference* depth_stencil_attachment,
                                         std::span<VkAttachmentReference> input_attachments, VkPipelineBindPoint pipeline_bind_point,
                                         VkSubpassDescriptionFlags flags, std::span<VkAttachmentReference> resolve_attachments,
                                         std::span<uint32_t> preserve_attachments) {
    VkSubpassDescription subpass_description{};
    subpass_description.colorAttachmentCount    = color_attachments.size();
    subpass_description.pColorAttachments       = color_attachments.data();
    subpass_description.pDepthStencilAttachment = depth_stencil_attachment;
    subpass_description.inputAttachmentCount    = input_attachments.size();
    subpass_description.pInputAttachments       = input_attachments.data();
    subpass_description.pipelineBindPoint       = pipeline_bind_point;
    subpass_description.flags                   = flags;
    subpass_description.pResolveAttachments     = resolve_attachments.data();
    subpass_description.preserveAttachmentCount = preserve_attachments.size();
    subpass_description.pPreserveAttachments    = preserve_attachments.data();

    return subpass_description;
}

VkSubpassDependency subpass_dependency(uint32_t src_subpass, uint32_t dst_subpass, VkPipelineStageFlags src_stage_mask,
                                       VkPipelineStageFlags dst_stage_mask, VkAccessFlags src_access_mask, VkAccessFlags dst_access_mask,
                                       VkDependencyFlags dependency_flags) {
    VkSubpassDependency subpass_dependency{};
    subpass_dependency.srcSubpass      = src_subpass;
    subpass_dependency.dstSubpass      = dst_subpass;
    subpass_dependency.srcStageMask    = src_stage_mask;
    subpass_dependency.dstStageMask    = dst_stage_mask;
    subpass_dependency.srcAccessMask   = src_access_mask;
    subpass_dependency.dstAccessMask   = dst_access_mask;
    subpass_dependency.dependencyFlags = dependency_flags;

    return subpass_dependency;
}

VkRenderPassCreateInfo render_pass_create_info(std::span<VkAttachmentDescription> attachments, std::span<VkSubpassDescription> subpasses,
                                               std::span<VkSubpassDependency> dependencies, VkRenderPassCreateFlags flags) {
    VkRenderPassCreateInfo render_pass_create_info{};
    render_pass_create_info.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    render_pass_create_info.attachmentCount = attachments.size();
    render_pass_create_info.pAttachments    = attachments.data();
    render_pass_create_info.subpassCount    = subpasses.size();
    render_pass_create_info.pSubpasses      = subpasses.data();
    render_pass_create_info.dependencyCount = dependencies.size();
    render_pass_create_info.pDependencies   = dependencies.data();
    render_pass_create_info.flags           = flags;

    return render_pass_create_info;
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