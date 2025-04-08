
#include <vk_lib/shaders.h>

namespace vk_lib {

VkShaderModuleCreateInfo shader_module_create_info(const uint32_t* code, uint32_t code_size, const void* pNext) {
    VkShaderModuleCreateInfo shader_module_create_info{};
    shader_module_create_info.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shader_module_create_info.codeSize = code_size;
    shader_module_create_info.pCode    = code;
    shader_module_create_info.pNext    = pNext;

    return shader_module_create_info;
}

VkShaderCreateInfoEXT shader_create_info(const void* code, uint32_t code_size, VkShaderCodeTypeEXT code_type, VkShaderStageFlagBits stage,
                                         VkShaderStageFlags next_stage, VkShaderCreateFlagsEXT flags,
                                         std::span<const VkDescriptorSetLayout> set_layouts,
                                         std::span<const VkPushConstantRange> push_constant_ranges, const VkSpecializationInfo* specialization_info,
                                         const char* entry_point, const void* pNext) {
    VkShaderCreateInfoEXT shader_create_info{};
    shader_create_info.sType                  = VK_STRUCTURE_TYPE_SHADER_CREATE_INFO_EXT;
    shader_create_info.pCode                  = code;
    shader_create_info.codeSize               = code_size;
    shader_create_info.codeType               = code_type;
    shader_create_info.stage                  = stage;
    shader_create_info.nextStage              = next_stage;
    shader_create_info.flags                  = flags;
    shader_create_info.setLayoutCount         = set_layouts.size();
    shader_create_info.pSetLayouts            = set_layouts.data();
    shader_create_info.pushConstantRangeCount = push_constant_ranges.size();
    shader_create_info.pPushConstantRanges    = push_constant_ranges.data();
    shader_create_info.pSpecializationInfo    = specialization_info;
    shader_create_info.pName                  = entry_point;
    shader_create_info.pNext                  = pNext;

    return shader_create_info;
}

} // namespace vk_lib