
#include <vk_lib/shader_data.h>

namespace vk_lib {

VkPushConstantRange push_constant_range(VkShaderStageFlags shader_stage_flags, uint32_t size, uint32_t offset) {
    VkPushConstantRange push_constant_range{};
    push_constant_range.stageFlags = shader_stage_flags;
    push_constant_range.offset     = offset;
    push_constant_range.size       = size;

    return push_constant_range;
}

VkSpecializationMapEntry specialization_map_entry(uint32_t constant_id, size_t size, uint32_t offset) {
    VkSpecializationMapEntry specialization_map_entry{};
    specialization_map_entry.constantID = constant_id;
    specialization_map_entry.offset     = offset;
    specialization_map_entry.size       = size;

    return specialization_map_entry;
}

VkSpecializationInfo specialization_info(const void* data, uint32_t data_size, std::span<VkSpecializationMapEntry> map_entries) {
    VkSpecializationInfo specialization_info{};
    specialization_info.mapEntryCount = map_entries.size();
    specialization_info.pMapEntries   = map_entries.data();
    specialization_info.dataSize      = data_size;
    specialization_info.pData         = data;

    return specialization_info;
}

VkDescriptorSetLayoutBinding descriptor_set_layout_binding(uint32_t binding, VkDescriptorType type, uint32_t descriptor_count,
                                                           VkShaderStageFlags stages, const VkSampler* immutable_sampler) {
    VkDescriptorSetLayoutBinding layout_binding{};
    layout_binding.binding            = binding;
    layout_binding.descriptorType     = type;
    layout_binding.stageFlags         = stages;
    layout_binding.descriptorCount    = descriptor_count;
    layout_binding.pImmutableSamplers = immutable_sampler;

    return layout_binding;
}

VkDescriptorSetLayoutCreateInfo descriptor_set_layout_create_info(std::span<VkDescriptorSetLayoutBinding> layout_bindings,
                                                                  VkDescriptorSetLayoutCreateFlags flags, const void* pNext) {
    VkDescriptorSetLayoutCreateInfo descriptor_set_layout_create_info{};
    descriptor_set_layout_create_info.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptor_set_layout_create_info.bindingCount = layout_bindings.size();
    descriptor_set_layout_create_info.pBindings    = layout_bindings.data();
    descriptor_set_layout_create_info.flags        = flags;
    descriptor_set_layout_create_info.pNext        = pNext;

    return descriptor_set_layout_create_info;
}

VkDescriptorPoolSize descriptor_pool_size(VkDescriptorType type, uint32_t descriptor_count) {
    VkDescriptorPoolSize descriptor_pool_size{};
    descriptor_pool_size.type            = type;
    descriptor_pool_size.descriptorCount = descriptor_count;

    return descriptor_pool_size;
}

VkDescriptorPoolCreateInfo descriptor_pool_create_info(uint32_t max_sets, std::span<VkDescriptorPoolSize> pool_sizes,
                                                       VkDescriptorPoolCreateFlags flags, const void* pNext) {
    VkDescriptorPoolCreateInfo descriptor_pool_create_info{};
    descriptor_pool_create_info.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptor_pool_create_info.maxSets       = max_sets;
    descriptor_pool_create_info.pPoolSizes    = pool_sizes.data();
    descriptor_pool_create_info.poolSizeCount = pool_sizes.size();
    descriptor_pool_create_info.flags         = flags;
    descriptor_pool_create_info.pNext         = pNext;

    return descriptor_pool_create_info;
}

VkDescriptorSetAllocateInfo descriptor_set_allocate_info(const VkDescriptorSetLayout* set_layout, VkDescriptorPool descriptor_pool,
                                                         uint32_t descriptor_set_count, const void* pNext) {
    VkDescriptorSetAllocateInfo descriptor_set_allocate_info{};
    descriptor_set_allocate_info.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    descriptor_set_allocate_info.pSetLayouts        = set_layout;
    descriptor_set_allocate_info.descriptorSetCount = descriptor_set_count;
    descriptor_set_allocate_info.descriptorPool     = descriptor_pool;
    descriptor_set_allocate_info.pNext              = pNext;

    return descriptor_set_allocate_info;
}

VkCopyDescriptorSet copy_descriptor_set_create(VkDescriptorSet src_set, VkDescriptorSet dst_set, uint32_t src_binding, uint32_t dst_binding,
                                               uint32_t src_array_element, uint32_t dst_array_element, uint32_t descriptor_count) {
    VkCopyDescriptorSet copy_descriptor_set{};
    copy_descriptor_set.sType           = VK_STRUCTURE_TYPE_COPY_DESCRIPTOR_SET;
    copy_descriptor_set.srcSet          = src_set;
    copy_descriptor_set.srcBinding      = src_binding;
    copy_descriptor_set.srcArrayElement = src_array_element;
    copy_descriptor_set.dstSet          = dst_set;
    copy_descriptor_set.dstBinding      = dst_binding;
    copy_descriptor_set.dstArrayElement = dst_array_element;
    copy_descriptor_set.descriptorCount = descriptor_count;

    return copy_descriptor_set;
}

VkDescriptorImageInfo descriptor_image_info(VkImageView image_view, VkImageLayout image_layout, VkSampler sampler) {
    VkDescriptorImageInfo image_info{};
    image_info.imageView   = image_view;
    image_info.sampler     = sampler;
    image_info.imageLayout = image_layout;

    return image_info;
}

VkDescriptorBufferInfo descriptor_buffer_info(VkBuffer buffer, uint64_t offset, uint64_t range) {
    VkDescriptorBufferInfo buffer_info{};
    buffer_info.buffer = buffer;
    buffer_info.offset = offset;
    buffer_info.range  = range;

    return buffer_info;
}

VkWriteDescriptorSet write_descriptor_set(uint32_t binding, VkDescriptorType type, VkDescriptorSet descriptor_set,
                                          const VkDescriptorImageInfo* image_info, const VkDescriptorBufferInfo* buffer_info,
                                          const VkBufferView* texel_buffer_view, uint32_t array_element, uint32_t descriptor_count,
                                          const void* pNext) {
    VkWriteDescriptorSet write_descriptor_set{};
    write_descriptor_set.sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write_descriptor_set.dstBinding       = binding;
    write_descriptor_set.dstSet           = descriptor_set;
    write_descriptor_set.descriptorCount  = descriptor_count;
    write_descriptor_set.descriptorType   = type;
    write_descriptor_set.pImageInfo       = image_info;
    write_descriptor_set.pBufferInfo      = buffer_info;
    write_descriptor_set.pTexelBufferView = texel_buffer_view;
    write_descriptor_set.dstArrayElement  = array_element;
    write_descriptor_set.pNext            = pNext;

    return write_descriptor_set;
}

VkWriteDescriptorSetInlineUniformBlockEXT write_descriptor_set_inline_uniform_block(uint32_t data_size, const void* data, const void* pNext) {
    VkWriteDescriptorSetInlineUniformBlockEXT inline_uniform_block_write{};
    inline_uniform_block_write.sType    = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_INLINE_UNIFORM_BLOCK_EXT;
    inline_uniform_block_write.dataSize = data_size;
    inline_uniform_block_write.pData    = data;
    inline_uniform_block_write.pNext    = pNext;

    return inline_uniform_block_write;
}

VkWriteDescriptorSetAccelerationStructureKHR
write_descriptor_set_acceleration_structure_khr_batch(std::span<VkAccelerationStructureKHR> acceleration_structures, const void* pNext) {
    VkWriteDescriptorSetAccelerationStructureKHR accel_struct_write{};
    accel_struct_write.sType                      = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_ACCELERATION_STRUCTURE_KHR;
    accel_struct_write.accelerationStructureCount = acceleration_structures.size();
    accel_struct_write.pAccelerationStructures    = acceleration_structures.data();
    accel_struct_write.pNext                      = pNext;

    return accel_struct_write;
}

VkWriteDescriptorSetAccelerationStructureKHR write_descriptor_set_acceleration_structure_khr(const VkAccelerationStructureKHR* acceleration_structure,
                                                                                             const void*                       pNext) {
    VkWriteDescriptorSetAccelerationStructureKHR accel_struct_write{};
    accel_struct_write.sType                      = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_ACCELERATION_STRUCTURE_KHR;
    accel_struct_write.accelerationStructureCount = 1;
    accel_struct_write.pAccelerationStructures    = acceleration_structure;
    accel_struct_write.pNext                      = pNext;

    return accel_struct_write;
}

} // namespace vk_lib