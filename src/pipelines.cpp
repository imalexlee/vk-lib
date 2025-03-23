#include <vk_lib/pipelines.h>

void pipeline_builder_add_shader_stage(GraphicsPipelineBuilder* builder, VkShaderStageFlagBits stage, VkShaderModule shader_module,
                                       VkPipelineShaderStageCreateFlags flags, const char* entry_point,
                                       const VkSpecializationInfo* specialization_info, const void* pNext) {
    builder->shader_stages.emplace_back(VkPipelineShaderStageCreateInfo{
        .sType               = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .stage               = stage,
        .flags               = flags,
        .module              = shader_module,
        .pName               = entry_point,
        .pSpecializationInfo = specialization_info,
        .pNext               = pNext,
    });
}

void graphics_pipeline_builder_set_vertex_input_state(GraphicsPipelineBuilder* builder, std::span<VkVertexInputBindingDescription> bindings,
                                                      std::span<VkVertexInputAttributeDescription> attributes, const void* pNext) {
    builder->vertex_input_state.vertexBindingDescriptionCount   = bindings.size();
    builder->vertex_input_state.pVertexBindingDescriptions      = bindings.data();
    builder->vertex_input_state.vertexAttributeDescriptionCount = attributes.size();
    builder->vertex_input_state.pVertexAttributeDescriptions    = attributes.data();
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

    builder->viewport_state.viewportCount = viewports.size();
    builder->viewport_state.pViewports    = viewports.data();
    builder->viewport_state.scissorCount  = scissors.size();
    builder->viewport_state.pScissors     = scissors.data();
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

void pipeline_builder_set_multisample_state(GraphicsPipelineBuilder* builder, uint32_t samples, bool sample_shading_enable, float min_sample_shading,
                                            const VkSampleMask* sample_mask, bool alpha_to_coverage_enable, bool alpha_to_one_enable) {
    builder->multisample_state.rasterizationSamples  = static_cast<VkSampleCountFlagBits>(samples);
    builder->multisample_state.sampleShadingEnable   = sample_shading_enable;
    builder->multisample_state.minSampleShading      = min_sample_shading;
    builder->multisample_state.pSampleMask           = sample_mask;
    builder->multisample_state.alphaToCoverageEnable = alpha_to_coverage_enable;
    builder->multisample_state.alphaToOneEnable      = alpha_to_one_enable;
}
