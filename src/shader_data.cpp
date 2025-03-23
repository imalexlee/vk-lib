
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

void descriptor_layout_builder_add_binding(DescriptorLayoutBuilder* builder, uint32_t binding, VkDescriptorType type, VkShaderStageFlags stage,
                                           uint32_t descriptor_count, const VkSampler* immutable_sampler) {
    VkDescriptorSetLayoutBinding layout_binding{};
    layout_binding.binding            = binding;
    layout_binding.descriptorType     = type;
    layout_binding.descriptorCount    = descriptor_count;
    layout_binding.stageFlags         = stage;
    layout_binding.pImmutableSamplers = immutable_sampler;

    builder->bindings.push_back(layout_binding);
}

void descriptor_layout_builder_clear(DescriptorLayoutBuilder* builder) { *builder = DescriptorLayoutBuilder{}; }

void descriptor_set_layout_destroy(VkDevice device, VkDescriptorSetLayout descriptor_set_layout) {
    vkDestroyDescriptorSetLayout(device, descriptor_set_layout, nullptr);
}

VkResult command_pool_reset(VkDevice device, VkCommandPool command_pool, VkCommandPoolResetFlags flags) {
    return vkResetCommandPool(device, command_pool, flags);
}

VkResult descriptor_layout_builder_layout_create(const DescriptorLayoutBuilder* layout_builder, VkDevice device,
                                                 VkDescriptorSetLayout* descriptor_set_layout, VkDescriptorSetLayoutCreateFlags flags,
                                                 const void* pNext) {
    VkDescriptorSetLayoutCreateInfo set_layout_create_info{};
    set_layout_create_info.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    set_layout_create_info.bindingCount = layout_builder->bindings.size();
    set_layout_create_info.pBindings    = layout_builder->bindings.data();
    set_layout_create_info.flags        = flags;
    set_layout_create_info.pNext        = pNext;

    return vkCreateDescriptorSetLayout(device, &set_layout_create_info, nullptr, descriptor_set_layout);
}

VkDescriptorPoolSize descriptor_pool_size_create(VkDescriptorType type, uint32_t descriptor_count) {
    VkDescriptorPoolSize descriptor_pool_size{};
    descriptor_pool_size.type            = type;
    descriptor_pool_size.descriptorCount = descriptor_count;

    return descriptor_pool_size;
}

VkResult descriptor_pool_create(VkDevice device, uint32_t max_descriptor_sets, std::span<VkDescriptorPoolSize> descriptor_pool_sizes,
                                VkDescriptorPool* descriptor_pool, VkDescriptorPoolCreateFlags flags, const void* pNext) {
    VkDescriptorPoolCreateInfo pool_ci{};
    pool_ci.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_ci.maxSets       = max_descriptor_sets;
    pool_ci.pPoolSizes    = descriptor_pool_sizes.data();
    pool_ci.poolSizeCount = descriptor_pool_sizes.size();
    pool_ci.flags         = flags;
    pool_ci.pNext         = pNext;

    return vkCreateDescriptorPool(device, &pool_ci, nullptr, descriptor_pool);
}

VkResult descriptor_pool_reset(VkDevice device, VkDescriptorPool descriptor_pool) { return vkResetDescriptorPool(device, descriptor_pool, 0); }

void descriptor_pool_destroy(VkDevice device, VkDescriptorPool descriptor_pool) { vkDestroyDescriptorPool(device, descriptor_pool, nullptr); }

VkResult descriptor_set_allocate(VkDevice device, VkDescriptorPool descriptor_pool, VkDescriptorSetLayout descriptor_set_layout,
                                 VkDescriptorSet* descriptor_set, const void* pNext) {
    VkDescriptorSetAllocateInfo descriptor_set_allocate_info{};
    descriptor_set_allocate_info.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    descriptor_set_allocate_info.pSetLayouts        = &descriptor_set_layout;
    descriptor_set_allocate_info.descriptorSetCount = 1;
    descriptor_set_allocate_info.descriptorPool     = descriptor_pool;
    descriptor_set_allocate_info.pNext              = pNext;

    return vkAllocateDescriptorSets(device, &descriptor_set_allocate_info, descriptor_set);
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

void descriptor_writer_write_image(DescriptorWriter* writer, uint32_t binding, VkImageView image_view, VkImageLayout image_layout,
                                   VkDescriptorType type, VkSampler sampler, uint32_t array_element) {

    const VkDescriptorImageInfo* image_info =
        &writer->image_infos.emplace_back(VkDescriptorImageInfo{.sampler = sampler, .imageView = image_view, .imageLayout = image_layout});

    VkWriteDescriptorSet write{};
    write.sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.dstBinding      = binding;
    write.dstSet          = nullptr;
    write.descriptorCount = 1;
    write.descriptorType  = type;
    write.pImageInfo      = image_info;
    write.dstArrayElement = array_element;

    writer->writes.push_back(write);
}

void descriptor_writer_write_buffer(DescriptorWriter* writer, uint32_t binding, VkBuffer buffer, uint64_t offset, uint64_t size,
                                    VkDescriptorType type, uint32_t array_element) {
    const VkDescriptorBufferInfo* buffer_info =
        &writer->buffer_infos.emplace_back(VkDescriptorBufferInfo{.buffer = buffer, .offset = offset, .range = size});

    VkWriteDescriptorSet write = {};
    write.sType                = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.dstBinding           = binding;
    write.dstSet               = nullptr;
    write.descriptorCount      = 1;
    write.descriptorType       = type;
    write.pBufferInfo          = buffer_info;
    write.dstArrayElement      = array_element;

    writer->writes.push_back(write);
}

void descriptor_writer_write_acceleration_structure_khr(DescriptorWriter* writer, uint32_t binding,
                                                        const VkAccelerationStructureKHR* acceleration_structure, uint32_t array_element) {
    const VkWriteDescriptorSetAccelerationStructureKHR* accel_struct_write =
        &writer->accel_struct_writes_khr.emplace_back(VkWriteDescriptorSetAccelerationStructureKHR{
            .sType                      = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_ACCELERATION_STRUCTURE_KHR,
            .accelerationStructureCount = 1,
            .pAccelerationStructures    = acceleration_structure,
        });

    VkWriteDescriptorSet write{};
    write.sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.dstBinding      = binding;
    write.dstSet          = nullptr;
    write.descriptorCount = 1;
    write.descriptorType  = VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR;
    write.dstArrayElement = array_element;
    write.pNext           = accel_struct_write;

    writer->writes.push_back(write);
}

void descriptor_writer_write_acceleration_structure_nv(DescriptorWriter* writer, uint32_t binding,
                                                       const VkAccelerationStructureNV* acceleration_structure, uint32_t array_element) {
    const VkWriteDescriptorSetAccelerationStructureNV* accel_struct_write =
        &writer->accel_struct_writes_nv.emplace_back(VkWriteDescriptorSetAccelerationStructureNV{
            .sType                      = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_ACCELERATION_STRUCTURE_NV,
            .accelerationStructureCount = 1,
            .pAccelerationStructures    = acceleration_structure,
        });

    VkWriteDescriptorSet write{};
    write.sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.dstBinding      = binding;
    write.dstSet          = nullptr;
    write.descriptorCount = 1;
    write.descriptorType  = VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_NV;
    write.dstArrayElement = array_element;
    write.pNext           = accel_struct_write;

    writer->writes.push_back(write);
}

void descriptor_writer_write_extension_descriptor(DescriptorWriter* writer, uint32_t binding, const void* pNext, VkDescriptorType type,
                                                  uint32_t array_element) {
    VkWriteDescriptorSet write{};
    write.sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.dstBinding      = binding;
    write.dstSet          = nullptr;
    write.descriptorCount = 1;
    write.descriptorType  = type;
    write.dstArrayElement = array_element;
    write.pNext           = pNext;

    writer->writes.push_back(write);
}

void descriptor_writer_clear(DescriptorWriter* writer) { *writer = DescriptorWriter{}; }

void desc_writer_update_descriptor_set(DescriptorWriter* writer, VkDevice device, VkDescriptorSet set,
                                       std::span<VkCopyDescriptorSet> descriptor_copies) {
    for (VkWriteDescriptorSet& write : writer->writes) {
        write.dstSet = set;
    }

    vkUpdateDescriptorSets(device, writer->writes.size(), writer->writes.data(), descriptor_copies.size(), descriptor_copies.data());
}