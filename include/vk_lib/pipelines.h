/*
 * Utilities regarding pipeline creation (graphics, compute, and ray tracing)
 */

#pragma once
#include <vk_lib/common.h>

[[nodiscard]] VkStencilOpState stencil_op_state_create(VkStencilOp fail_op, VkStencilOp pass_op, VkStencilOp depth_fail_op, VkCompareOp compare_op,
                                                       uint32_t compare_mask, uint32_t write_mask, uint32_t reference);

VkResult pipeline_layout_create(VkDevice device, std::span<VkDescriptorSetLayout> set_layouts, std::span<VkPushConstantRange> push_constant_ranges,
                                VkPipelineLayout* pipeline_layout, VkPipelineLayoutCreateFlags flags = 0, const void* pNext = nullptr);

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
                                                VkPipelineShaderStageCreateFlags flags = 0, const char* entry_point = "main",
                                                const VkSpecializationInfo* specialization_info = nullptr, const void* pNext = nullptr);

void graphics_pipeline_builder_clear_shader_stages(GraphicsPipelineBuilder* builder);

void graphics_pipeline_builder_set_vertex_input_state(GraphicsPipelineBuilder* builder, std::span<VkVertexInputBindingDescription> bindings,
                                                      std::span<VkVertexInputAttributeDescription> attributes, const void* pNext = nullptr);

void graphics_pipeline_builder_set_input_assembly_state(GraphicsPipelineBuilder* builder, VkPrimitiveTopology topology,
                                                        bool primitive_restart_enable = false);

void graphics_pipeline_builder_set_tesselation_state(GraphicsPipelineBuilder* builder, uint32_t patch_control_points, const void* pNext = nullptr);

void graphics_pipeline_builder_set_viewports(GraphicsPipelineBuilder* builder, std::span<VkViewport> viewports, std::span<VkRect2D> scissors,
                                             const void* pNext = nullptr);

void graphics_pipeline_builder_set_viewport(GraphicsPipelineBuilder* builder, const VkViewport* viewport, const VkRect2D* scissor, const void* pNext);

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

void graphics_pipeline_builder_add_color_blend_attachment(GraphicsPipelineBuilder* builder, VkBlendFactor src_color_blend_factor,
                                                          VkBlendFactor dst_color_blend_factor, VkBlendOp color_blend_op,
                                                          VkBlendFactor src_alpha_blend_factor, VkBlendFactor dst_alpha_blend_factor,
                                                          VkBlendOp alpha_blend_op, VkColorComponentFlags color_write_mask);

void graphics_pipeline_set_dynamic_states(GraphicsPipelineBuilder* builder, std::span<VkDynamicState> dynamic_states);

void graphics_pipeline_builder_set_pNext(GraphicsPipelineBuilder* builder, const void* pNext);

void graphics_pipeline_builder_set_flags(GraphicsPipelineBuilder* builder, VkPipelineCreateFlags flags);

void graphics_pipeline_builder_set_layout(GraphicsPipelineBuilder* builder, VkPipelineLayout pipeline_layout);

void graphics_pipeline_builder_set_render_pass(GraphicsPipelineBuilder* builder, VkRenderPass render_pass, uint32_t subpass_index = 0);

void graphics_pipeline_builder_set_base_pipeline(GraphicsPipelineBuilder* builder, VkPipeline base_pipeline, int32_t base_pipeline_index = 0);

VkResult graphics_pipeline_builder_pipeline_create(const GraphicsPipelineBuilder* builder, VkDevice device, VkPipeline* graphics_pipeline,
                                                   VkPipelineCache pipeline_cache = nullptr);
