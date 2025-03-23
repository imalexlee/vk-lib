/*
 * Utilities regarding shader data binding with descriptors, push constants, and specialization constants
 */

#pragma once
#include <vk_lib/common.h>

// size and offset must be multiples of 4
[[nodiscard]] VkPushConstantRange push_constant_range_create(VkShaderStageFlags shader_stage_flags, uint32_t offset, uint32_t size);

[[nodiscard]] VkSpecializationMapEntry specialization_map_entry_create(uint32_t constant_id, uint32_t offset, size_t size);

[[nodiscard]] VkSpecializationInfo specialization_info_create(const void* data, uint32_t data_size, std::span<VkSpecializationMapEntry> map_entries);

struct DescriptorLayoutBuilder {
    std::vector<VkDescriptorSetLayoutBinding> bindings;
};

void descriptor_layout_builder_add_binding(DescriptorLayoutBuilder* builder, uint32_t binding, VkDescriptorType type, VkShaderStageFlags stage,
                                           uint32_t descriptor_count = 1, const VkSampler* immutable_sampler = nullptr);

void descriptor_layout_builder_clear(DescriptorLayoutBuilder* builder);

VkResult descriptor_layout_builder_layout_create(const DescriptorLayoutBuilder* layout_builder, VkDevice device,
                                                 VkDescriptorSetLayout* descriptor_set_layout, VkDescriptorSetLayoutCreateFlags flags = 0,
                                                 const void* pNext = nullptr);

void descriptor_set_layout_destroy(VkDevice device, VkDescriptorSetLayout descriptor_set_layout);

[[nodiscard]] VkDescriptorPoolSize descriptor_pool_size_create(VkDescriptorType type, uint32_t descriptor_count);

VkResult descriptor_pool_create(VkDevice device, uint32_t max_descriptor_sets, std::span<VkDescriptorPoolSize> descriptor_pool_sizes,
                                VkDescriptorPool* descriptor_pool, VkDescriptorPoolCreateFlags flags = 0, const void* pNext = nullptr);

VkResult descriptor_pool_reset(VkDevice device, VkDescriptorPool descriptor_pool);

void descriptor_pool_destroy(VkDevice device, VkDescriptorPool descriptor_pool);

VkResult descriptor_set_allocate(VkDevice device, VkDescriptorPool descriptor_pool, VkDescriptorSetLayout descriptor_set_layout,
                                 VkDescriptorSet* descriptor_set, const void* pNext = nullptr);

[[nodiscard]] VkCopyDescriptorSet copy_descriptor_set_create(VkDescriptorSet src_set, VkDescriptorSet dst_set, uint32_t src_binding,
                                                             uint32_t dst_binding, uint32_t src_array_element = 0, uint32_t dst_array_element = 0,
                                                             uint32_t descriptor_count = 1);

struct DescriptorWriter {
    std::deque<VkDescriptorImageInfo>                        image_infos;
    std::deque<VkDescriptorBufferInfo>                       buffer_infos;
    std::deque<VkWriteDescriptorSetAccelerationStructureKHR> accel_struct_writes_khr;
    std::deque<VkWriteDescriptorSetAccelerationStructureNV>  accel_struct_writes_nv;

    std::vector<VkWriteDescriptorSet> writes;
};

void descriptor_writer_write_image(DescriptorWriter* writer, uint32_t binding, VkImageView image_view, VkImageLayout image_layout,
                                   VkDescriptorType type, VkSampler sampler, uint32_t array_element = 0);

void descriptor_writer_write_buffer(DescriptorWriter* writer, uint32_t binding, VkBuffer buffer, uint64_t offset, uint64_t size,
                                    VkDescriptorType type, uint32_t array_element = 0);

void descriptor_writer_write_acceleration_structure_khr(DescriptorWriter* writer, uint32_t binding,
                                                        const VkAccelerationStructureKHR* acceleration_structure, uint32_t array_element = 0);

void descriptor_writer_write_acceleration_structure_nv(DescriptorWriter* writer, uint32_t binding,
                                                       const VkAccelerationStructureNV* acceleration_structure, uint32_t array_element = 0);

void descriptor_writer_write_extension_descriptor(DescriptorWriter* writer, uint32_t binding, const void* pNext, VkDescriptorType type,
                                                  uint32_t array_element = 0);

void descriptor_writer_clear(DescriptorWriter* writer);

void desc_writer_update_descriptor_set(DescriptorWriter* writer, VkDevice device, VkDescriptorSet set,
                                       std::span<VkCopyDescriptorSet> descriptor_copies = {});
