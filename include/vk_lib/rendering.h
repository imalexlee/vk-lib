/*
 * Utilities regarding rendering
 */

#pragma once
#include <vk_lib/common.h>

namespace vk_lib {

[[nodiscard]] VkViewport viewport(float width, float height, float x_offset = 0, float y_offset = 0, float min_depth = 0, float max_depth = 1);

[[nodiscard]] VkAttachmentDescription attachment_description(VkFormat format, VkImageLayout initial_layout, VkImageLayout final_layout,
                                                             VkAttachmentLoadOp           load_op          = VK_ATTACHMENT_LOAD_OP_CLEAR,
                                                             VkAttachmentStoreOp          store_op         = VK_ATTACHMENT_STORE_OP_STORE,
                                                             VkAttachmentLoadOp           stencil_load_op  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                                             VkAttachmentStoreOp          stencil_store_op = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                                             VkSampleCountFlagBits        samples          = VK_SAMPLE_COUNT_1_BIT,
                                                             VkAttachmentDescriptionFlags flags            = 0);

[[nodiscard]] VkAttachmentReference attachment_reference(uint32_t attachment, VkImageLayout layout);

[[nodiscard]] VkSubpassDescription
subpass_description(std::span<const VkAttachmentReference> color_attachments, const VkAttachmentReference* depth_stencil_attachment = nullptr,
                    std::span<const VkAttachmentReference> input_attachments = {},
                    VkPipelineBindPoint pipeline_bind_point = VK_PIPELINE_BIND_POINT_GRAPHICS, VkSubpassDescriptionFlags flags = 0,
                    std::span<const VkAttachmentReference> resolve_attachments = {}, std::span<const uint32_t> preserve_attachments = {});

[[nodiscard]] VkSubpassDependency subpass_dependency(uint32_t src_subpass, uint32_t dst_subpass,
                                                     VkPipelineStageFlags src_stage_mask   = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
                                                     VkPipelineStageFlags dst_stage_mask   = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
                                                     VkAccessFlags        src_access_mask  = VK_ACCESS_MEMORY_WRITE_BIT,
                                                     VkAccessFlags        dst_access_mask  = VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT,
                                                     VkDependencyFlags    dependency_flags = 0);

[[nodiscard]] VkRenderPassCreateInfo render_pass_create_info(std::span<const VkAttachmentDescription> attachments,
                                                             std::span<const VkSubpassDescription>    subpasses,
                                                             std::span<const VkSubpassDependency>     dependencies = {},
                                                             VkRenderPassCreateFlags                  flags        = 0);

/*
 * CORE EXTENSIONS
 */

// VULKAN 1.3

[[nodiscard]] VkRenderingAttachmentInfoKHR rendering_attachment_info(VkImageView image_view, VkImageLayout image_layout, VkAttachmentLoadOp load_op,
                                                                     VkAttachmentStoreOp store_op, const VkClearValue* clear_value = nullptr,
                                                                     VkResolveModeFlagBitsKHR resolve_mode         = VK_RESOLVE_MODE_NONE_KHR,
                                                                     VkImageView              resolve_image_view   = nullptr,
                                                                     VkImageLayout            resolve_image_layout = VK_IMAGE_LAYOUT_UNDEFINED);

[[nodiscard]] VkRenderingInfoKHR rendering_info(VkRect2D render_area, std::span<const VkRenderingAttachmentInfoKHR> color_attachments,
                                                const VkRenderingAttachmentInfoKHR* depth_attachment   = nullptr,
                                                const VkRenderingAttachmentInfoKHR* stencil_attachment = nullptr, VkRenderingFlagsKHR flags = 0,
                                                uint32_t view_mask = 0, uint32_t layer_count = 1, const void* pNext = nullptr);
} // namespace vk_lib