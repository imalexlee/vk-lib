#include <vk_lib/pipelines.h>

VkStencilOpState stencil_op_state_create(VkStencilOp fail_op, VkStencilOp pass_op, VkStencilOp depth_fail_op, VkCompareOp compare_op,
                                         uint32_t compare_mask, uint32_t write_mask, uint32_t reference) {
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

void graphics_pipeline_builder_add_shader_stage(GraphicsPipelineBuilder* builder, VkShaderStageFlagBits stage, VkShaderModule shader_module,
                                                VkPipelineShaderStageCreateFlags flags, const char* entry_point,
                                                const VkSpecializationInfo* specialization_info, const void* pNext) {
    builder->shader_stages.emplace_back(VkPipelineShaderStageCreateInfo{
        .sType               = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .pNext               = pNext,
        .flags               = flags,
        .stage               = stage,
        .module              = shader_module,
        .pName               = entry_point,
        .pSpecializationInfo = specialization_info,
    });
    builder->graphics_pipeline_create_info.stageCount = builder->shader_stages.size();
    builder->graphics_pipeline_create_info.pStages    = builder->shader_stages.data();
}

void graphics_pipeline_builder_clear_shader_stages(GraphicsPipelineBuilder* builder) {
    builder->shader_stages.clear();

    builder->graphics_pipeline_create_info.stageCount = 0;
    builder->graphics_pipeline_create_info.pStages    = nullptr;
}

void graphics_pipeline_builder_set_vertex_input_state(GraphicsPipelineBuilder* builder, std::span<VkVertexInputBindingDescription> bindings,
                                                      std::span<VkVertexInputAttributeDescription> attributes, const void* pNext) {
    builder->vertex_binding_descriptions.assign(bindings.begin(), bindings.end());
    builder->vertex_attribute_descriptions.assign(attributes.begin(), attributes.end());

    builder->vertex_input_state.vertexBindingDescriptionCount   = builder->vertex_binding_descriptions.size();
    builder->vertex_input_state.pVertexBindingDescriptions      = builder->vertex_binding_descriptions.data();
    builder->vertex_input_state.vertexAttributeDescriptionCount = builder->vertex_attribute_descriptions.size();
    builder->vertex_input_state.pVertexAttributeDescriptions    = builder->vertex_attribute_descriptions.data();
    builder->vertex_input_state.pNext                           = pNext;
}

void graphics_pipeline_builder_set_input_assembly_state(GraphicsPipelineBuilder* builder, VkPrimitiveTopology topology,
                                                        bool primitive_restart_enable) {
    builder->input_assembly_state.topology               = topology;
    builder->input_assembly_state.primitiveRestartEnable = primitive_restart_enable;
}

void graphics_pipeline_builder_set_tesselation_state(GraphicsPipelineBuilder* builder, uint32_t patch_control_points, const void* pNext) {
    builder->tessellation_state.patchControlPoints = patch_control_points;
    builder->tessellation_state.pNext              = pNext;
}

void graphics_pipeline_builder_set_viewports(GraphicsPipelineBuilder* builder, std::span<VkViewport> viewports, std::span<VkRect2D> scissors,
                                             const void* pNext) {
    builder->viewports.assign(viewports.begin(), viewports.end());
    builder->scissors.assign(scissors.begin(), scissors.end());

    builder->viewport_state.viewportCount = builder->viewports.size();
    builder->viewport_state.pViewports    = builder->viewports.data();
    builder->viewport_state.scissorCount  = builder->scissors.size();
    builder->viewport_state.pScissors     = builder->scissors.data();
    builder->viewport_state.pNext         = pNext;
}

void graphics_pipeline_builder_set_viewport(GraphicsPipelineBuilder* builder, const VkViewport* viewport, const VkRect2D* scissor,
                                            const void* pNext) {
    builder->viewport_state.viewportCount = 1;
    builder->viewport_state.pViewports    = viewport;
    builder->viewport_state.scissorCount  = 1;
    builder->viewport_state.pScissors     = scissor;
    builder->viewport_state.pNext         = pNext;
}

void graphics_pipeline_builder_set_rasterization_state(GraphicsPipelineBuilder* builder, VkPolygonMode polygon_mode, VkCullModeFlags cull_mode,
                                                       VkFrontFace front_face, float line_width, bool depth_clamp_enable,
                                                       bool rasterizer_discard_enable, float depth_bias_constant_factor, float depth_bias_clamp,
                                                       float depth_bias_slope_factor, const void* pNext) {
    builder->rasterization_state.polygonMode             = polygon_mode;
    builder->rasterization_state.cullMode                = cull_mode;
    builder->rasterization_state.frontFace               = front_face;
    builder->rasterization_state.lineWidth               = line_width;
    builder->rasterization_state.depthClampEnable        = depth_clamp_enable;
    builder->rasterization_state.rasterizerDiscardEnable = rasterizer_discard_enable;
    builder->rasterization_state.depthBiasConstantFactor = depth_bias_constant_factor;
    builder->rasterization_state.depthBiasClamp          = depth_bias_clamp;
    builder->rasterization_state.depthBiasSlopeFactor    = depth_bias_slope_factor;
    builder->rasterization_state.pNext                   = pNext;
}

void graphics_pipeline_builder_set_multisample_state(GraphicsPipelineBuilder* builder, uint32_t samples, bool sample_shading_enable,
                                                     float min_sample_shading, const VkSampleMask* sample_mask, bool alpha_to_coverage_enable,
                                                     bool alpha_to_one_enable) {
    builder->multisample_state.rasterizationSamples  = static_cast<VkSampleCountFlagBits>(samples);
    builder->multisample_state.sampleShadingEnable   = sample_shading_enable;
    builder->multisample_state.minSampleShading      = min_sample_shading;
    builder->multisample_state.pSampleMask           = sample_mask;
    builder->multisample_state.alphaToCoverageEnable = alpha_to_coverage_enable;
    builder->multisample_state.alphaToOneEnable      = alpha_to_one_enable;
}

void graphics_pipeline_builder_set_depth_stencil_state(GraphicsPipelineBuilder* builder, bool depth_test_enable, bool depth_write_enable,
                                                       VkCompareOp depth_compare_op, bool depth_bounds_test_enable, bool stencil_test_enable,
                                                       const VkStencilOpState* front, const VkStencilOpState* back, float min_depth_bounds,
                                                       float max_depth_bounds) {
    builder->depth_stencil_state.depthTestEnable       = depth_test_enable;
    builder->depth_stencil_state.depthWriteEnable      = depth_write_enable;
    builder->depth_stencil_state.depthCompareOp        = depth_compare_op;
    builder->depth_stencil_state.depthBoundsTestEnable = depth_bounds_test_enable;
    builder->depth_stencil_state.stencilTestEnable     = stencil_test_enable;
    builder->depth_stencil_state.front                 = *front;
    builder->depth_stencil_state.back                  = *back;
    builder->depth_stencil_state.minDepthBounds        = min_depth_bounds;
    builder->depth_stencil_state.maxDepthBounds        = max_depth_bounds;
}

void graphics_pipeline_builder_set_color_blend_state(GraphicsPipelineBuilder* builder, bool logic_op_enable, VkLogicOp logic_op,
                                                     std::array<float, 4> blend_constants, VkPipelineColorBlendStateCreateFlags flags,
                                                     const void* pNext) {
    builder->color_blend_state.logicOpEnable     = logic_op_enable;
    builder->color_blend_state.logicOp           = logic_op;
    builder->color_blend_state.flags             = flags;
    builder->color_blend_state.blendConstants[0] = blend_constants[0];
    builder->color_blend_state.blendConstants[1] = blend_constants[1];
    builder->color_blend_state.blendConstants[2] = blend_constants[2];
    builder->color_blend_state.blendConstants[3] = blend_constants[3];
    builder->color_blend_state.pNext             = pNext;
}

void graphics_pipeline_builder_add_color_blend_attachment(GraphicsPipelineBuilder* builder, VkBlendFactor src_color_blend_factor,
                                                          VkBlendFactor dst_color_blend_factor, VkBlendOp color_blend_op,
                                                          VkBlendFactor src_alpha_blend_factor, VkBlendFactor dst_alpha_blend_factor,
                                                          VkBlendOp alpha_blend_op, VkColorComponentFlags color_write_mask) {
    builder->color_blend_attachments.emplace_back(VkPipelineColorBlendAttachmentState{
        .blendEnable         = true,
        .srcColorBlendFactor = src_color_blend_factor,
        .dstColorBlendFactor = dst_color_blend_factor,
        .colorBlendOp        = color_blend_op,
        .srcAlphaBlendFactor = src_alpha_blend_factor,
        .dstAlphaBlendFactor = dst_alpha_blend_factor,
        .alphaBlendOp        = alpha_blend_op,
        .colorWriteMask      = color_write_mask,
    });

    builder->color_blend_state.attachmentCount = builder->color_blend_attachments.size();
    builder->color_blend_state.pAttachments    = builder->color_blend_attachments.data();
}

void graphics_pipeline_set_dynamic_states(GraphicsPipelineBuilder* builder, std::span<VkDynamicState> dynamic_states) {
    builder->dynamic_states.assign(dynamic_states.begin(), dynamic_states.end());

    builder->dynamic_state.dynamicStateCount = builder->dynamic_states.size();
    builder->dynamic_state.pDynamicStates    = builder->dynamic_states.data();
}

void graphics_pipeline_builder_set_pNext(GraphicsPipelineBuilder* builder, const void* pNext) {
    builder->graphics_pipeline_create_info.pNext = pNext;
}

void graphics_pipeline_builder_set_layout(GraphicsPipelineBuilder* builder, VkPipelineLayout pipeline_layout) {
    builder->graphics_pipeline_create_info.layout = pipeline_layout;
}

void graphics_pipeline_builder_set_flags(GraphicsPipelineBuilder* builder, VkPipelineCreateFlags flags) {
    builder->graphics_pipeline_create_info.flags = flags;
}

void graphics_pipeline_builder_set_render_pass(GraphicsPipelineBuilder* builder, VkRenderPass render_pass, uint32_t subpass_index) {
    builder->graphics_pipeline_create_info.renderPass = render_pass;
    builder->graphics_pipeline_create_info.subpass    = subpass_index;
}

void graphics_pipeline_builder_set_base_pipeline(GraphicsPipelineBuilder* builder, VkPipeline base_pipeline, int32_t base_pipeline_index) {
    builder->graphics_pipeline_create_info.basePipelineHandle = base_pipeline;
    builder->graphics_pipeline_create_info.basePipelineIndex  = base_pipeline_index;
}

void graphics_pipeline_builder_clear(GraphicsPipelineBuilder* builder) { *builder = GraphicsPipelineBuilder{}; }

VkResult graphics_pipeline_builder_pipeline_create(const GraphicsPipelineBuilder* builder, VkDevice device, VkPipeline* graphics_pipeline,
                                                   VkPipelineCache pipeline_cache) {
    VkGraphicsPipelineCreateInfo graphics_pipeline_create_info = builder->graphics_pipeline_create_info;
    graphics_pipeline_create_info.pVertexInputState            = &builder->vertex_input_state;
    graphics_pipeline_create_info.pInputAssemblyState          = &builder->input_assembly_state;
    graphics_pipeline_create_info.pTessellationState           = &builder->tessellation_state;
    graphics_pipeline_create_info.pViewportState               = &builder->viewport_state;
    graphics_pipeline_create_info.pRasterizationState          = &builder->rasterization_state;
    graphics_pipeline_create_info.pMultisampleState            = &builder->multisample_state;
    graphics_pipeline_create_info.pDepthStencilState           = &builder->depth_stencil_state;
    graphics_pipeline_create_info.pColorBlendState             = &builder->color_blend_state;
    graphics_pipeline_create_info.pDynamicState                = &builder->dynamic_state;

    return vkCreateGraphicsPipelines(device, pipeline_cache, 1, &graphics_pipeline_create_info, nullptr, graphics_pipeline);
}
