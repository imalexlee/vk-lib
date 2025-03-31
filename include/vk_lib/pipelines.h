/*
 * Utilities regarding pipeline creation (graphics, compute, and ray tracing)
 */

#pragma once
#include <vk_lib/common.h>

[[nodiscard]] VkPipelineShaderStageCreateInfo pipeline_shader_stage_create_info(VkShaderStageFlagBits stage, VkShaderModule shader_module,
                                                                                VkPipelineShaderStageCreateFlags flags               = 0,
                                                                                const char*                      entry_point         = "main",
                                                                                const VkSpecializationInfo*      specialization_info = nullptr,
                                                                                const void*                      pNext               = nullptr);

[[nodiscard]] VkVertexInputBindingDescription vertex_input_binding_description(uint32_t binding, uint32_t stride, VkVertexInputRate input_rate);

[[nodiscard]] VkVertexInputAttributeDescription vertex_input_attribute_description(uint32_t location, uint32_t binding, VkFormat format,
                                                                                   uint32_t offset);

[[nodiscard]] VkPipelineVertexInputStateCreateInfo pipeline_vertex_input_state_create_info(std::span<VkVertexInputBindingDescription>   bindings,
                                                                                           std::span<VkVertexInputAttributeDescription> attributes,
                                                                                           const void* pNext = nullptr);

[[nodiscard]] VkPipelineInputAssemblyStateCreateInfo pipeline_input_assembly_state_create_info(VkPrimitiveTopology topology,
                                                                                               bool                primitive_restart_enabled = false);

[[nodiscard]] VkPipelineTessellationStateCreateInfo pipeline_tessellation_state_create_info(uint32_t    patch_control_points,
                                                                                            const void* pNext = nullptr);

[[nodiscard]] VkPipelineViewportStateCreateInfo pipeline_multi_viewport_state_create_info(std::span<VkViewport> viewports,
                                                                                          std::span<VkRect2D> scissors, const void* pNext = nullptr);

[[nodiscard]] VkPipelineViewportStateCreateInfo pipeline_viewport_state_create_info(const VkViewport* viewport, const VkRect2D* scissor,
                                                                                    const void* pNext = nullptr);

[[nodiscard]] VkPipelineRasterizationStateCreateInfo
rasterization_state_create_info(VkPolygonMode polygon_mode, VkCullModeFlags cull_mode, VkFrontFace front_face, float line_width = 1.0f,
                                bool depth_clamp_enable = false, bool rasterizer_discard_enable = false, float depth_bias_constant_factor = 0.0f,
                                float depth_bias_clamp = 0.0f, float depth_bias_slope_factor = 0.0f, const void* pNext = nullptr);

// TODO: add multisample state create info

[[nodiscard]] VkStencilOpState stencil_op_state_create(VkStencilOp fail_op, VkStencilOp pass_op, VkStencilOp depth_fail_op, VkCompareOp compare_op,
                                                       uint32_t compare_mask, uint32_t write_mask, uint32_t reference);

VkResult pipeline_layout_create(VkDevice device, std::span<VkDescriptorSetLayout> set_layouts, std::span<VkPushConstantRange> push_constant_ranges,
                                VkPipelineLayout* pipeline_layout, VkPipelineLayoutCreateFlags flags = 0, const void* pNext = nullptr);

void pipeline_layout_destroy(VkDevice device, VkPipelineLayout pipeline_layout);

struct GraphicsPipelineBuilder {
    VkGraphicsPipelineCreateInfo                 graphics_pipeline_create_info{VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO};
    std::vector<VkPipelineShaderStageCreateInfo> shader_stages{};
    VkPipelineVertexInputStateCreateInfo         vertex_input_state{VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO};
    VkPipelineInputAssemblyStateCreateInfo       input_assembly_state{VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO};
    VkPipelineTessellationStateCreateInfo        tessellation_state{VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO};
    VkPipelineViewportStateCreateInfo            viewport_state{VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO};
    VkPipelineRasterizationStateCreateInfo       rasterization_state{VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO};
    VkPipelineMultisampleStateCreateInfo         multisample_state{VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO};
    VkPipelineDepthStencilStateCreateInfo        depth_stencil_state{VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO};
    VkPipelineColorBlendStateCreateInfo          color_blend_state{VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO};
    VkPipelineDynamicStateCreateInfo             dynamic_state{VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO};

    std::vector<VkVertexInputBindingDescription>     vertex_binding_descriptions;
    std::vector<VkVertexInputAttributeDescription>   vertex_attribute_descriptions;
    std::vector<VkViewport>                          viewports;
    std::vector<VkRect2D>                            scissors;
    std::vector<VkPipelineColorBlendAttachmentState> color_blend_attachments;
    std::vector<VkDynamicState>                      dynamic_states;
};

void graphics_pipeline_builder_add_shader_stage(GraphicsPipelineBuilder* builder, VkShaderStageFlagBits stage, VkShaderModule shader_module,
                                                VkPipelineShaderStageCreateFlags flags = 0, std::string_view entry_point = "main",
                                                const VkSpecializationInfo* specialization_info = nullptr, const void* pNext = nullptr);

void graphics_pipeline_builder_clear_shader_stages(GraphicsPipelineBuilder* builder);

void graphics_pipeline_builder_set_vertex_input_state(GraphicsPipelineBuilder* builder, std::span<VkVertexInputBindingDescription> bindings,
                                                      std::span<VkVertexInputAttributeDescription> attributes, const void* pNext = nullptr);

void graphics_pipeline_builder_set_input_assembly_state(GraphicsPipelineBuilder* builder, VkPrimitiveTopology topology,
                                                        bool primitive_restart_enable = false);

void graphics_pipeline_builder_set_tesselation_state(GraphicsPipelineBuilder* builder, uint32_t patch_control_points, const void* pNext = nullptr);

void graphics_pipeline_builder_set_viewports(GraphicsPipelineBuilder* builder, std::span<VkViewport> viewports, std::span<VkRect2D> scissors,
                                             const void* pNext = nullptr);

void graphics_pipeline_builder_set_viewport(GraphicsPipelineBuilder* builder, const VkViewport* viewport, const VkRect2D* scissor,
                                            const void* pNext = nullptr);

void graphics_pipeline_builder_set_rasterization_state(GraphicsPipelineBuilder* builder, VkPolygonMode polygon_mode, VkCullModeFlags cull_mode,
                                                       VkFrontFace front_face, float line_width = 1.0f, bool depth_clamp_enable = false,
                                                       bool rasterizer_discard_enable = false, float depth_bias_constant_factor = 0.0f,
                                                       float depth_bias_clamp = 0.0f, float depth_bias_slope_factor = 0.0f,
                                                       const void* pNext = nullptr);

void graphics_pipeline_builder_set_multisample_state(GraphicsPipelineBuilder* builder, VkSampleCountFlagBits samples,
                                                     bool sample_shading_enable = false, float min_sample_shading = 1.0f,
                                                     const VkSampleMask* sample_mask = nullptr, bool alpha_to_coverage_enable = false,
                                                     bool alpha_to_one_enable = false);

void graphics_pipeline_builder_set_depth_stencil_state(GraphicsPipelineBuilder* builder, bool depth_test_enable, bool depth_write_enable,
                                                       VkCompareOp depth_compare_op, bool depth_bounds_test_enable = false,
                                                       bool stencil_test_enable = false, const VkStencilOpState* front = nullptr,
                                                       const VkStencilOpState* back = nullptr, float min_depth_bounds = 0.0f,
                                                       float max_depth_bounds = 1.0f);

void graphics_pipeline_builder_set_color_blend_state(GraphicsPipelineBuilder* builder, bool logic_op_enable, VkLogicOp logic_op = VK_LOGIC_OP_COPY,
                                                     std::array<float, 4>                 blend_constants = {0, 0, 0, 0},
                                                     VkPipelineColorBlendStateCreateFlags flags = 0, const void* pNext = nullptr);

void graphics_pipeline_builder_add_color_blend_attachment(
    GraphicsPipelineBuilder* builder, bool blend_enabled, VkBlendFactor src_color_blend_factor = VK_BLEND_FACTOR_ONE,
    VkBlendFactor dst_color_blend_factor = VK_BLEND_FACTOR_ZERO, VkBlendOp color_blend_op = VK_BLEND_OP_ADD,
    VkBlendFactor src_alpha_blend_factor = VK_BLEND_FACTOR_ONE, VkBlendFactor dst_alpha_blend_factor = VK_BLEND_FACTOR_ZERO,
    VkBlendOp             alpha_blend_op   = VK_BLEND_OP_ADD,
    VkColorComponentFlags color_write_mask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
                                             VK_COLOR_COMPONENT_A_BIT);

void graphics_pipeline_set_dynamic_states(GraphicsPipelineBuilder* builder, std::span<VkDynamicState> dynamic_states);

void graphics_pipeline_builder_set_pNext(GraphicsPipelineBuilder* builder, const void* pNext);

void graphics_pipeline_builder_set_flags(GraphicsPipelineBuilder* builder, VkPipelineCreateFlags flags);

void graphics_pipeline_builder_set_layout(GraphicsPipelineBuilder* builder, VkPipelineLayout pipeline_layout);

void graphics_pipeline_builder_set_render_pass(GraphicsPipelineBuilder* builder, VkRenderPass render_pass, uint32_t subpass_index = 0);

void graphics_pipeline_builder_set_base_pipeline(GraphicsPipelineBuilder* builder, VkPipeline base_pipeline, int32_t base_pipeline_index = 0);

VkResult graphics_pipeline_builder_pipeline_create(const GraphicsPipelineBuilder* builder, VkDevice device, VkPipeline* graphics_pipeline,
                                                   VkPipelineCache pipeline_cache = nullptr);

void pipeline_destroy(VkDevice device, VkPipeline pipeline);

/*
 * CORE EXTENSIONS
 */

// VULKAN 1.3

[[nodiscard]] VkPipelineRenderingCreateInfoKHR rendering_create_info_create(std::span<VkFormat> color_attachment_formats,
                                                                            VkFormat            depth_attachment_format   = VK_FORMAT_UNDEFINED,
                                                                            VkFormat            stencil_attachment_format = VK_FORMAT_UNDEFINED,
                                                                            uint32_t view_mask = 0, const void* pNext = nullptr);