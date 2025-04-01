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

[[nodiscard]] VkPipelineMultisampleStateCreateInfo
pipeline_multisample_state_create_info(VkSampleCountFlagBits samples, bool sample_shading_enable = false, float min_sample_shading = 1.0f,
                                       const VkSampleMask* sample_mask = nullptr, bool alpha_to_coverage_enable = false,
                                       bool alpha_to_one_enable = false);

[[nodiscard]] VkStencilOpState stencil_op_state(VkStencilOp fail_op, VkStencilOp pass_op, VkStencilOp depth_fail_op, VkCompareOp compare_op,
                                                uint32_t compare_mask, uint32_t write_mask, uint32_t reference);

[[nodiscard]] VkPipelineDepthStencilStateCreateInfo
pipeline_depth_stencil_state_create_info(bool depth_test_enable, bool depth_write_enable, VkCompareOp depth_compare_op,
                                         bool depth_bounds_test_enable = false, bool stencil_test_enable = false,
                                         const VkStencilOpState* front = nullptr, const VkStencilOpState* back = nullptr,
                                         float min_depth_bounds = 0.0f, float max_depth_bounds = 1.0f);

[[nodiscard]] VkPipelineColorBlendStateCreateInfo pipeline_color_blend_state_create_info(bool logic_op_enable, VkLogicOp logic_op = VK_LOGIC_OP_COPY,
                                                                                         std::array<float, 4> blend_constants       = {0, 0, 0, 0},
                                                                                         VkPipelineColorBlendStateCreateFlags flags = 0,
                                                                                         const void*                          pNext = nullptr);

[[nodiscard]] VkPipelineDynamicStateCreateInfo pipeline_dynamic_state_create_info(std::span<VkDynamicState> dynamic_states);

[[nodiscard]] VkPipelineLayoutCreateInfo pipeline_layout_create_info(std::span<VkDescriptorSetLayout> set_layouts,
                                                                     std::span<VkPushConstantRange>   push_constant_ranges,
                                                                     VkPipelineLayout* pipeline_layout, VkPipelineLayoutCreateFlags flags = 0,
                                                                     const void* pNext = nullptr);

[[nodiscard]] VkGraphicsPipelineCreateInfo graphics_pipeline_create_info(
    VkPipelineLayout layout, VkRenderPass render_pass, std::span<VkPipelineShaderStageCreateInfo> shader_stages,
    const VkPipelineVertexInputStateCreateInfo* vertex_input_state, const VkPipelineInputAssemblyStateCreateInfo* input_assembly_state,
    const VkPipelineViewportStateCreateInfo* viewport_state, const VkPipelineRasterizationStateCreateInfo* rasterization_state,
    const VkPipelineMultisampleStateCreateInfo* multisample_state, const VkPipelineDepthStencilStateCreateInfo* depth_stencil_state,
    const VkPipelineColorBlendStateCreateInfo* color_blend_state, const VkPipelineDynamicStateCreateInfo* dynamic_state = nullptr,
    const VkPipelineTessellationStateCreateInfo* tessellation_state = nullptr, VkPipelineCreateFlags flags = 0, uint32_t subpass_index = 0,
    VkPipeline base_pipeline = nullptr, int32_t base_pipeline_index = 0, const void* pNext = nullptr);

/*
 * CORE EXTENSIONS
 */

// VULKAN 1.3

[[nodiscard]] VkPipelineRenderingCreateInfoKHR pipeline_rendering_create_info(std::span<VkFormat> color_attachment_formats,
                                                                              VkFormat            depth_attachment_format   = VK_FORMAT_UNDEFINED,
                                                                              VkFormat            stencil_attachment_format = VK_FORMAT_UNDEFINED,
                                                                              uint32_t view_mask = 0, const void* pNext = nullptr);