/*
* Utilities regarding shader data binding with descriptors, push constants, and specialization constants
*/

#pragma once
#include <vk_lib/common.h>

// size and offset must be multiples of 4
[[nodiscard]] VkPushConstantRange push_constant_range_create(
    VkShaderStageFlags shader_stage_flags,
    uint32_t offset, uint32_t size);

[[nodiscard]] VkSpecializationMapEntry specialization_map_entry_create(
    uint32_t constant_id, uint32_t offset,
    size_t size);

[[nodiscard]] VkSpecializationInfo specialization_info_create(const void* data, uint32_t data_size,
                                                              std::span<VkSpecializationMapEntry>
                                                              map_entries);

