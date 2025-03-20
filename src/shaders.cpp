
#include <vk_lib/shaders.h>

VkResult shader_module_create(VkDevice device, const uint32_t* code, size_t code_size, VkShaderModule* shader_module, const void* pNext) {
    VkShaderModuleCreateInfo shader_module_create_info{};
    shader_module_create_info.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shader_module_create_info.codeSize = code_size;
    shader_module_create_info.pCode    = code;
    shader_module_create_info.pNext    = pNext;

    return vkCreateShaderModule(device, &shader_module_create_info, nullptr, shader_module);
}

void shader_module_destroy(VkDevice device, VkShaderModule shader_module) {
    vkDestroyShaderModule(device, shader_module, nullptr);
}

void shader_object_builder_set_stage(ShaderObjectBuilder* builder, VkShaderStageFlagBits stage, VkShaderStageFlags next_stage, VkShaderCreateFlagsEXT create_flags) {
    builder->shader_create_info.stage     = stage;
    builder->shader_create_info.nextStage = next_stage;
    builder->shader_create_info.flags     = create_flags;
}

void shader_object_builder_set_code(ShaderObjectBuilder* builder, const void* code, size_t code_size, VkShaderCodeTypeEXT code_type, const char* entry_point_name) {
    builder->shader_code.resize(code_size);
    std::memcpy(builder->shader_code.data(), code, code_size);

    builder->entry_point_name = entry_point_name;

    builder->shader_create_info.pCode    = builder->shader_code.data();
    builder->shader_create_info.codeSize = code_size;
    builder->shader_create_info.codeType = code_type;
    builder->shader_create_info.pName    = builder->entry_point_name.data();
}

void shader_object_builder_set_descriptor_set_layouts(ShaderObjectBuilder* builder, std::span<VkDescriptorSetLayout> descriptor_set_layouts) {
    builder->descriptor_set_layouts.assign(descriptor_set_layouts.begin(), descriptor_set_layouts.end());
    builder->shader_create_info.pSetLayouts    = builder->descriptor_set_layouts.data();
    builder->shader_create_info.setLayoutCount = builder->descriptor_set_layouts.size();
}

void shader_object_builder_set_push_constant_ranges(ShaderObjectBuilder* builder, std::span<VkPushConstantRange> push_constant_ranges) {
    builder->push_constant_ranges.assign(push_constant_ranges.begin(), push_constant_ranges.end());
    builder->shader_create_info.pPushConstantRanges    = builder->push_constant_ranges.data();
    builder->shader_create_info.pushConstantRangeCount = builder->push_constant_ranges.size();
}

void shader_object_builder_set_specialization_info(ShaderObjectBuilder* builder, const VkSpecializationInfo* specialization_info) {
    builder->specialization_info = *specialization_info;
}

void shader_object_builder_set_pNext(ShaderObjectBuilder* builder, const void* pNext) {
    builder->shader_create_info.pNext = pNext;
}

void shader_object_builder_clear(ShaderObjectBuilder* builder) {
    *builder = ShaderObjectBuilder{};
}

VkResult shader_object_builder_shader_create(const ShaderObjectBuilder* builder, VkDevice device, VkShaderEXT* shader_object) {
    return vkCreateShadersEXT(device, 1, &builder->shader_create_info, nullptr, shader_object);
}

void shader_object_destroy(VkDevice device, VkShaderEXT shader) {
    vkDestroyShaderEXT(device, shader, nullptr);
}