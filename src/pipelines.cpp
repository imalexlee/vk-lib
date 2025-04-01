#include <vk_lib/pipelines.h>

VkPipelineShaderStageCreateInfo pipeline_shader_stage_create_info(VkShaderStageFlagBits stage, VkShaderModule shader_module,
                                                                  VkPipelineShaderStageCreateFlags flags, const char* entry_point,
                                                                  const VkSpecializationInfo* specialization_info, const void* pNext) {
    VkPipelineShaderStageCreateInfo shader_stage_create_info{};
    shader_stage_create_info.sType               = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shader_stage_create_info.flags               = flags;
    shader_stage_create_info.stage               = stage;
    shader_stage_create_info.module              = shader_module;
    shader_stage_create_info.pName               = entry_point;
    shader_stage_create_info.pSpecializationInfo = specialization_info;
    shader_stage_create_info.pNext               = pNext;

    return shader_stage_create_info;
}

VkVertexInputBindingDescription vertex_input_binding_description(uint32_t binding, uint32_t stride, VkVertexInputRate input_rate) {
    VkVertexInputBindingDescription input_binding_description{};
    input_binding_description.binding   = binding;
    input_binding_description.stride    = stride;
    input_binding_description.inputRate = input_rate;

    return input_binding_description;
}

VkVertexInputAttributeDescription vertex_input_attribute_description(uint32_t location, uint32_t binding, VkFormat format, uint32_t offset) {
    VkVertexInputAttributeDescription input_attribute_description{};
    input_attribute_description.location = location;
    input_attribute_description.binding  = binding;
    input_attribute_description.format   = format;
    input_attribute_description.offset   = offset;

    return input_attribute_description;
}

VkPipelineVertexInputStateCreateInfo pipeline_vertex_input_state_create_info(std::span<VkVertexInputBindingDescription>   bindings,
                                                                             std::span<VkVertexInputAttributeDescription> attributes,
                                                                             const void*                                  pNext) {
    VkPipelineVertexInputStateCreateInfo vertex_input_state_create_info{};
    vertex_input_state_create_info.sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertex_input_state_create_info.vertexBindingDescriptionCount   = bindings.size();
    vertex_input_state_create_info.pVertexBindingDescriptions      = bindings.data();
    vertex_input_state_create_info.vertexAttributeDescriptionCount = attributes.size();
    vertex_input_state_create_info.pVertexAttributeDescriptions    = attributes.data();
    vertex_input_state_create_info.pNext                           = pNext;

    return vertex_input_state_create_info;
}

VkPipelineInputAssemblyStateCreateInfo pipeline_input_assembly_state_create_info(VkPrimitiveTopology topology, bool primitive_restart_enabled) {
    VkPipelineInputAssemblyStateCreateInfo input_assembly_state_create_info{};
    input_assembly_state_create_info.sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    input_assembly_state_create_info.topology               = topology;
    input_assembly_state_create_info.primitiveRestartEnable = primitive_restart_enabled;

    return input_assembly_state_create_info;
}

VkPipelineTessellationStateCreateInfo pipeline_tessellation_state_create_info(uint32_t patch_control_points, const void* pNext) {
    VkPipelineTessellationStateCreateInfo tessellation_state_create_info{};
    tessellation_state_create_info.sType              = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
    tessellation_state_create_info.patchControlPoints = patch_control_points;
    tessellation_state_create_info.pNext              = pNext;

    return tessellation_state_create_info;
}

VkPipelineViewportStateCreateInfo pipeline_multi_viewport_state_create_info(std::span<VkViewport> viewports, std::span<VkRect2D> scissors,
                                                                            const void* pNext) {
    VkPipelineViewportStateCreateInfo viewport_state_create_info{};
    viewport_state_create_info.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewport_state_create_info.viewportCount = viewports.size();
    viewport_state_create_info.pViewports    = viewports.data();
    viewport_state_create_info.scissorCount  = scissors.size();
    viewport_state_create_info.pScissors     = scissors.data();
    viewport_state_create_info.pNext         = pNext;

    return viewport_state_create_info;
}

VkPipelineViewportStateCreateInfo pipeline_viewport_state_create_info(const VkViewport* viewport, const VkRect2D* scissor, const void* pNext) {
    VkPipelineViewportStateCreateInfo viewport_state_create_info{};
    viewport_state_create_info.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewport_state_create_info.viewportCount = 1;
    viewport_state_create_info.pViewports    = viewport;
    viewport_state_create_info.scissorCount  = 1;
    viewport_state_create_info.pScissors     = scissor;
    viewport_state_create_info.pNext         = pNext;

    return viewport_state_create_info;
}

VkPipelineRasterizationStateCreateInfo rasterization_state_create_info(VkPolygonMode polygon_mode, VkCullModeFlags cull_mode, VkFrontFace front_face,
                                                                       float line_width, bool depth_clamp_enable, bool rasterizer_discard_enable,
                                                                       float depth_bias_constant_factor, float depth_bias_clamp,
                                                                       float depth_bias_slope_factor, const void* pNext) {
    VkPipelineRasterizationStateCreateInfo rasterization_state_create_info{};
    rasterization_state_create_info.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterization_state_create_info.polygonMode             = polygon_mode;
    rasterization_state_create_info.cullMode                = cull_mode;
    rasterization_state_create_info.frontFace               = front_face;
    rasterization_state_create_info.lineWidth               = line_width;
    rasterization_state_create_info.depthClampEnable        = depth_clamp_enable;
    rasterization_state_create_info.rasterizerDiscardEnable = rasterizer_discard_enable;
    rasterization_state_create_info.depthBiasConstantFactor = depth_bias_constant_factor;
    rasterization_state_create_info.depthBiasClamp          = depth_bias_clamp;
    rasterization_state_create_info.depthBiasSlopeFactor    = depth_bias_slope_factor;
    rasterization_state_create_info.pNext                   = pNext;

    return rasterization_state_create_info;
}

VkPipelineMultisampleStateCreateInfo pipeline_multisample_state_create_info(VkSampleCountFlagBits samples, bool sample_shading_enable,
                                                                            float min_sample_shading, const VkSampleMask* sample_mask,
                                                                            bool alpha_to_coverage_enable, bool alpha_to_one_enable) {
    VkPipelineMultisampleStateCreateInfo multisample_state_create_info{};
    multisample_state_create_info.sType                 = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisample_state_create_info.rasterizationSamples  = samples;
    multisample_state_create_info.sampleShadingEnable   = sample_shading_enable;
    multisample_state_create_info.minSampleShading      = min_sample_shading;
    multisample_state_create_info.pSampleMask           = sample_mask;
    multisample_state_create_info.alphaToCoverageEnable = alpha_to_coverage_enable;
    multisample_state_create_info.alphaToOneEnable      = alpha_to_one_enable;

    return multisample_state_create_info;
}

VkStencilOpState stencil_op_state(VkStencilOp fail_op, VkStencilOp pass_op, VkStencilOp depth_fail_op, VkCompareOp compare_op, uint32_t compare_mask,
                                  uint32_t write_mask, uint32_t reference) {
    VkStencilOpState stencil_op_state{};
    stencil_op_state.failOp      = fail_op;
    stencil_op_state.passOp      = pass_op;
    stencil_op_state.depthFailOp = depth_fail_op;
    stencil_op_state.compareOp   = compare_op;
    stencil_op_state.compareMask = compare_mask;
    stencil_op_state.writeMask   = write_mask;
    stencil_op_state.reference   = reference;

    return stencil_op_state;
}

VkPipelineDepthStencilStateCreateInfo pipeline_depth_stencil_state_create_info(bool depth_test_enable, bool depth_write_enable,
                                                                               VkCompareOp depth_compare_op, bool depth_bounds_test_enable,
                                                                               bool stencil_test_enable, const VkStencilOpState* front,
                                                                               const VkStencilOpState* back, float min_depth_bounds,
                                                                               float max_depth_bounds) {
    VkPipelineDepthStencilStateCreateInfo depth_stencil_state_create_info{};
    depth_stencil_state_create_info.sType                 = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depth_stencil_state_create_info.depthTestEnable       = depth_test_enable;
    depth_stencil_state_create_info.depthWriteEnable      = depth_write_enable;
    depth_stencil_state_create_info.depthCompareOp        = depth_compare_op;
    depth_stencil_state_create_info.depthBoundsTestEnable = depth_bounds_test_enable;
    depth_stencil_state_create_info.stencilTestEnable     = stencil_test_enable;
    depth_stencil_state_create_info.front                 = *front;
    depth_stencil_state_create_info.back                  = *back;
    depth_stencil_state_create_info.minDepthBounds        = min_depth_bounds;
    depth_stencil_state_create_info.maxDepthBounds        = max_depth_bounds;

    return depth_stencil_state_create_info;
}

VkPipelineColorBlendStateCreateInfo pipeline_color_blend_state_create_info(bool logic_op_enable, VkLogicOp logic_op,
                                                                           std::array<float, 4>                 blend_constants,
                                                                           VkPipelineColorBlendStateCreateFlags flags, const void* pNext) {
    VkPipelineColorBlendStateCreateInfo color_blend_state_create_info{};
    color_blend_state_create_info.sType             = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    color_blend_state_create_info.logicOpEnable     = logic_op_enable;
    color_blend_state_create_info.logicOp           = logic_op;
    color_blend_state_create_info.flags             = flags;
    color_blend_state_create_info.blendConstants[0] = blend_constants[0];
    color_blend_state_create_info.blendConstants[1] = blend_constants[1];
    color_blend_state_create_info.blendConstants[2] = blend_constants[2];
    color_blend_state_create_info.blendConstants[3] = blend_constants[3];
    color_blend_state_create_info.pNext             = pNext;

    return color_blend_state_create_info;
}

VkPipelineDynamicStateCreateInfo pipeline_dynamic_state_create_info(std::span<VkDynamicState> dynamic_states) {
    VkPipelineDynamicStateCreateInfo dynamic_state_create_info{};
    dynamic_state_create_info.sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamic_state_create_info.dynamicStateCount = dynamic_states.size();
    dynamic_state_create_info.pDynamicStates    = dynamic_states.data();

    return dynamic_state_create_info;
}

VkPipelineLayoutCreateInfo pipeline_layout_create_info(std::span<VkDescriptorSetLayout> set_layouts,
                                                       std::span<VkPushConstantRange> push_constant_ranges, VkPipelineLayout* pipeline_layout,
                                                       VkPipelineLayoutCreateFlags flags, const void* pNext) {
    VkPipelineLayoutCreateInfo pipeline_layout_create_info{};
    pipeline_layout_create_info.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipeline_layout_create_info.setLayoutCount         = set_layouts.size();
    pipeline_layout_create_info.pSetLayouts            = set_layouts.data();
    pipeline_layout_create_info.pushConstantRangeCount = push_constant_ranges.size();
    pipeline_layout_create_info.pPushConstantRanges    = push_constant_ranges.data();
    pipeline_layout_create_info.flags                  = flags;
    pipeline_layout_create_info.pNext                  = pNext;

    return pipeline_layout_create_info;
}

VkGraphicsPipelineCreateInfo graphics_pipeline_create_info(
    VkPipelineLayout layout, VkPipelineCreateFlags flags, std::span<VkPipelineShaderStageCreateInfo> shader_stages,
    const VkPipelineVertexInputStateCreateInfo* vertex_input_state, const VkPipelineInputAssemblyStateCreateInfo* input_assembly_state,
    const VkPipelineTessellationStateCreateInfo* tessellation_state, const VkPipelineViewportStateCreateInfo* viewport_state,
    const VkPipelineRasterizationStateCreateInfo* rasterization_state, const VkPipelineMultisampleStateCreateInfo* multisample_state,
    const VkPipelineDepthStencilStateCreateInfo* depth_stencil_state, const VkPipelineColorBlendStateCreateInfo* color_blend_state,
    const VkPipelineDynamicStateCreateInfo* dynamic_state, VkRenderPass render_pass, uint32_t subpass_index, VkPipeline base_pipeline,
    int32_t base_pipeline_index, const void* pNext) {

    VkGraphicsPipelineCreateInfo graphics_pipeline_create_info{};
    graphics_pipeline_create_info.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    graphics_pipeline_create_info.flags               = flags;
    graphics_pipeline_create_info.stageCount          = shader_stages.size();
    graphics_pipeline_create_info.pStages             = shader_stages.data();
    graphics_pipeline_create_info.pVertexInputState   = vertex_input_state;
    graphics_pipeline_create_info.pInputAssemblyState = input_assembly_state;
    graphics_pipeline_create_info.pTessellationState  = tessellation_state;
    graphics_pipeline_create_info.pViewportState      = viewport_state;
    graphics_pipeline_create_info.pRasterizationState = rasterization_state;
    graphics_pipeline_create_info.pMultisampleState   = multisample_state;
    graphics_pipeline_create_info.pDepthStencilState  = depth_stencil_state;
    graphics_pipeline_create_info.pColorBlendState    = color_blend_state;
    graphics_pipeline_create_info.pDynamicState       = dynamic_state;
    graphics_pipeline_create_info.layout              = layout;
    graphics_pipeline_create_info.renderPass          = render_pass;
    graphics_pipeline_create_info.subpass             = subpass_index;
    graphics_pipeline_create_info.basePipelineHandle  = base_pipeline;
    graphics_pipeline_create_info.basePipelineIndex   = base_pipeline_index;
    graphics_pipeline_create_info.pNext               = pNext;

    return graphics_pipeline_create_info;
}

VkPipelineRenderingCreateInfoKHR pipeline_rendering_create_info(std::span<VkFormat> color_attachment_formats, VkFormat depth_attachment_format,
                                                                VkFormat stencil_attachment_format, uint32_t view_mask, const void* pNext) {
    VkPipelineRenderingCreateInfoKHR rendering_create_info{};
    rendering_create_info.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR;
    rendering_create_info.viewMask                = view_mask;
    rendering_create_info.colorAttachmentCount    = color_attachment_formats.size();
    rendering_create_info.pColorAttachmentFormats = color_attachment_formats.data();
    rendering_create_info.depthAttachmentFormat   = depth_attachment_format;
    rendering_create_info.stencilAttachmentFormat = stencil_attachment_format;
    rendering_create_info.pNext                   = pNext;

    return rendering_create_info;
}
