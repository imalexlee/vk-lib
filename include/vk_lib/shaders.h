/*
* Utilities regarding shader upload and creation
*/

#pragma once
#include <vk_lib/common.h>

VkResult shader_module_create(VkDevice device, const uint32_t* code, size_t code_size,
                              VkShaderModule* shader_module);

void shader_module_destroy(VkDevice device, VkShaderModule shader_module);

// Only works for unlinked shaders currently
struct ShaderObjectBuilder
{
    VkShaderCreateInfoEXT shader_create_info{};
    std::string entry_point_name{"main"};
    std::vector<VkDescriptorSetLayout> descriptor_set_layouts;
    std::vector<VkPushConstantRange> push_constant_ranges;
    std::vector<uint8_t> shader_code;
    VkSpecializationInfo specialization_info{};
};

void shader_object_builder_set_stage(ShaderObjectBuilder* builder, VkShaderStageFlagBits stage,
                                     VkShaderStageFlags next_stage = 0,
                                     VkShaderCreateFlagsEXT create_flags = 0);

void shader_object_builder_set_code(ShaderObjectBuilder* builder, const void* code,
                                    size_t code_size, VkShaderCodeTypeEXT code_type,
                                    const char* entry_point_name = "main");

void shader_object_builder_set_descriptor_set_layouts(ShaderObjectBuilder* builder,
                                                      std::span<VkDescriptorSetLayout>
                                                      descriptor_set_layouts);

void shader_object_builder_set_push_constant_ranges(ShaderObjectBuilder* builder,
                                                    std::span<VkPushConstantRange>
                                                    push_constant_ranges);

void shader_object_builder_set_specialization_info(ShaderObjectBuilder* builder,
                                                   const VkSpecializationInfo* specialization_info);

VkResult shader_object_builder_shader_create(const ShaderObjectBuilder* builder, VkDevice device,
                                             VkShaderEXT* shader_object);
