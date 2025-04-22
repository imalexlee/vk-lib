/*
 * Utilities regarding pipeline creation (graphics, compute, and ray tracing)
 */

#pragma once
#include <vk_lib/common.h>

namespace vk_lib {

[[nodiscard]] VkPipelineShaderStageCreateInfo pipeline_shader_stage_create_info(VkShaderStageFlagBits stage, VkShaderModule shader_module,
                                                                                VkPipelineShaderStageCreateFlags flags               = 0,
                                                                                const char*                      entry_point         = "main",
                                                                                const VkSpecializationInfo*      specialization_info = nullptr,
                                                                                const void*                      pNext               = nullptr);

[[nodiscard]] VkPipelineLayoutCreateInfo pipeline_layout_create_info(std::span<const VkDescriptorSetLayout> set_layouts          = {},
                                                                     std::span<const VkPushConstantRange>   push_constant_ranges = {},
                                                                     VkPipelineLayoutCreateFlags flags = 0, const void* pNext = nullptr);

[[nodiscard]] VkComputePipelineCreateInfo compute_pipeline_create_info(VkPipelineLayout layout, VkPipelineShaderStageCreateInfo stage,
                                                                       VkPipelineCreateFlags flags = 0, VkPipeline base_pipeline = nullptr,
                                                                       int32_t base_pipeline_index = 0, const void* pNext = nullptr);

[[nodiscard]] VkVertexInputBindingDescription vertex_input_binding_description(uint32_t binding, uint32_t stride, VkVertexInputRate input_rate);

[[nodiscard]] VkVertexInputAttributeDescription vertex_input_attribute_description(uint32_t binding, uint32_t location, VkFormat format,
                                                                                   uint32_t offset);

[[nodiscard]] VkPipelineVertexInputStateCreateInfo
pipeline_vertex_input_state_create_info(std::span<const VkVertexInputBindingDescription>   bindings   = {},
                                        std::span<const VkVertexInputAttributeDescription> attributes = {}, const void* pNext = nullptr);

[[nodiscard]] VkPipelineInputAssemblyStateCreateInfo pipeline_input_assembly_state_create_info(VkPrimitiveTopology topology,
                                                                                               bool                primitive_restart_enabled = false);

[[nodiscard]] VkPipelineTessellationStateCreateInfo pipeline_tessellation_state_create_info(uint32_t    patch_control_points,
                                                                                            const void* pNext = nullptr);

[[nodiscard]] VkPipelineViewportStateCreateInfo
pipeline_multi_viewport_state_create_info(std::span<const VkViewport> viewports, std::span<const VkRect2D> scissors, const void* pNext = nullptr);

[[nodiscard]] VkPipelineViewportStateCreateInfo pipeline_viewport_state_create_info(const VkViewport* viewport, const VkRect2D* scissor,
                                                                                    const void* pNext = nullptr);

[[nodiscard]] VkPipelineRasterizationStateCreateInfo
pipeline_rasterization_state_create_info(VkPolygonMode polygon_mode, VkFrontFace front_face, VkCullModeFlags cull_mode = VK_CULL_MODE_NONE,
                                         bool depth_bias_enable = false, float depth_bias_constant_factor = 0, float depth_bias_slope_factor = 0,
                                         bool depth_clamp_enable = false, float depth_bias_clamp = 0, float line_width = 1,
                                         bool rasterizer_discard_enable = false, const void* pNext = nullptr);

[[nodiscard]] VkPipelineMultisampleStateCreateInfo
pipeline_multisample_state_create_info(VkSampleCountFlagBits rasterization_samples, bool sample_shading_enable = false,
                                       float min_sample_shading = 1.0f, const VkSampleMask* sample_mask = nullptr,
                                       bool alpha_to_coverage_enable = false, bool alpha_to_one_enable = false);

[[nodiscard]] VkStencilOpState stencil_op_state(VkStencilOp fail_op, VkStencilOp pass_op, VkStencilOp depth_fail_op, VkCompareOp compare_op,
                                                uint32_t compare_mask, uint32_t write_mask, uint32_t reference);

[[nodiscard]] VkPipelineDepthStencilStateCreateInfo
pipeline_depth_stencil_state_create_info(bool depth_test_enable = false, bool depth_write_enable = false,
                                         VkCompareOp depth_compare_op = VK_COMPARE_OP_LESS_OR_EQUAL, bool depth_bounds_test_enable = false,
                                         bool stencil_test_enable = false, const VkStencilOpState* front = nullptr,
                                         const VkStencilOpState* back = nullptr, float min_depth_bounds = 0.0f, float max_depth_bounds = 1.0f);

[[nodiscard]] VkPipelineColorBlendAttachmentState
pipeline_color_blend_attachment_state(bool blend_enabled = false, VkBlendFactor src_color_blend_factor = VK_BLEND_FACTOR_SRC_ALPHA,
                                      VkBlendFactor dst_color_blend_factor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
                                      VkBlendOp color_blend_op = VK_BLEND_OP_ADD, VkBlendFactor src_alpha_blend_factor = VK_BLEND_FACTOR_ONE,
                                      VkBlendFactor dst_alpha_blend_factor = VK_BLEND_FACTOR_ZERO, VkBlendOp alpha_blend_op = VK_BLEND_OP_ADD,
                                      VkColorComponentFlags color_write_mask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                                                                               VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);

[[nodiscard]] VkPipelineColorBlendStateCreateInfo
pipeline_color_blend_state_create_info(std::span<const VkPipelineColorBlendAttachmentState> color_blend_attachment_states,
                                       bool logic_op_enable = false, VkLogicOp logic_op = VK_LOGIC_OP_COPY,
                                       std::array<float, 4> blend_constants = {0, 0, 0, 0}, VkPipelineColorBlendStateCreateFlags flags = 0,
                                       const void* pNext = nullptr);

[[nodiscard]] VkPipelineDynamicStateCreateInfo pipeline_dynamic_state_create_info(std::span<const VkDynamicState> dynamic_states);

[[nodiscard]] VkGraphicsPipelineCreateInfo graphics_pipeline_create_info(
    VkPipelineLayout layout, VkRenderPass render_pass, std::span<const VkPipelineShaderStageCreateInfo> shader_stages,
    const VkPipelineVertexInputStateCreateInfo* vertex_input_state, const VkPipelineInputAssemblyStateCreateInfo* input_assembly_state,
    const VkPipelineViewportStateCreateInfo* viewport_state, const VkPipelineRasterizationStateCreateInfo* rasterization_state,
    const VkPipelineMultisampleStateCreateInfo* multisample_state, const VkPipelineColorBlendStateCreateInfo* color_blend_state,
    const VkPipelineDepthStencilStateCreateInfo* depth_stencil_state = nullptr, const VkPipelineDynamicStateCreateInfo* dynamic_state = nullptr,
    const VkPipelineTessellationStateCreateInfo* tessellation_state = nullptr, VkPipelineCreateFlags flags = 0, uint32_t subpass_index = 0,
    VkPipeline base_pipeline = nullptr, int32_t base_pipeline_index = 0, const void* pNext = nullptr);

/*
 * CORE EXTENSIONS
 */

// VULKAN 1.3

[[nodiscard]] VkPipelineRenderingCreateInfoKHR pipeline_rendering_create_info(std::span<const VkFormat> color_attachment_formats,
                                                                              VkFormat                  depth_attachment_format = VK_FORMAT_UNDEFINED,
                                                                              VkFormat stencil_attachment_format                = VK_FORMAT_UNDEFINED,
                                                                              uint32_t view_mask = 0, const void* pNext = nullptr);

} // namespace vk_lib