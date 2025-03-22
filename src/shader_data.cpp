
#include <vk_lib/shader_data.h>

VkPushConstantRange push_constant_range_create(VkShaderStageFlags shader_stage_flags, uint32_t offset, uint32_t size) {
    VkPushConstantRange push_constant_range{};
    push_constant_range.stageFlags = shader_stage_flags;
    push_constant_range.offset     = offset;
    push_constant_range.size       = size;

    return push_constant_range;
}

VkSpecializationMapEntry specialization_map_entry_create(uint32_t constant_id, uint32_t offset, size_t size) {
    VkSpecializationMapEntry specialization_map_entry{};
    specialization_map_entry.constantID = constant_id;
    specialization_map_entry.offset     = offset;
    specialization_map_entry.size       = size;

    return specialization_map_entry;
}

VkSpecializationInfo specialization_info_create(const void* data, uint32_t data_size, std::span<VkSpecializationMapEntry> map_entries) {
    VkSpecializationInfo specialization_info{};
    specialization_info.mapEntryCount = map_entries.size();
    specialization_info.pMapEntries   = map_entries.data();
    specialization_info.dataSize      = data_size;
    specialization_info.pData         = data;

    return specialization_info;
}