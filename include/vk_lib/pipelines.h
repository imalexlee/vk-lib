/*
 * Utilities regarding pipeline creation (graphics, compute, and ray tracing)
 */

#pragma once
#include <vk_lib/common.h>

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
};

void graphics_pipeline_builder_add_shader_stage(GraphicsPipelineBuilder* builder, VkShaderStageFlagBits stage, VkShaderModule shader_module,
                                                VkPipelineShaderStageCreateFlags flags = 0, const char* entry_point = "main",
                                                const VkSpecializationInfo* specialization_info = nullptr, const void* pNext = nullptr);

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

void pipeline_builder_set_multisample_state(GraphicsPipelineBuilder* builder, VkSampleCountFlagBits samples, bool sample_shading_enable = false,
                                            float min_sample_shading = 1.0f, const VkSampleMask* sample_mask = nullptr,
                                            bool alpha_to_coverage_enable = false, bool alpha_to_one_enable = false);
