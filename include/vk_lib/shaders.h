/*
 * Utilities regarding shader upload and creation
 */

#pragma once
#include <vk_lib/common.h>

namespace vk_lib {

[[nodiscard]] VkShaderModuleCreateInfo shader_module_create_info(const uint32_t* code, uint32_t code_size, const void* pNext = nullptr);

/*
 * NON-CORE EXTENSIONS
 */

[[nodiscard]] VkShaderCreateInfoEXT shader_create_info(const void* code, uint32_t code_size, VkShaderCodeTypeEXT code_type,
                                                       VkShaderStageFlagBits stage, VkShaderStageFlags next_stage, VkShaderCreateFlagsEXT flags = 0,
                                                       std::span<const VkDescriptorSetLayout> set_layouts          = {},
                                                       std::span<const VkPushConstantRange>   push_constant_ranges = {},
                                                       const VkSpecializationInfo* specialization_info = nullptr, const char* entry_point = "main",
                                                       const void* pNext = nullptr);

} // namespace vk_lib