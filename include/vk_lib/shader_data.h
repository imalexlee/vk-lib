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

void descriptor_set_image_update(VkDevice device, VkDescriptorSet set, uint32_t binding, VkImageView image_view, VkImageLayout image_layout,
                                 VkDescriptorType type, VkSampler sampler = nullptr, uint32_t array_element = 0);

void descriptor_set_buffer_update(VkDevice device, VkDescriptorSet set, uint32_t binding, VkBuffer buffer, VkDescriptorType type, uint64_t offset = 0,
                                  uint64_t size = VK_WHOLE_SIZE, uint32_t array_element = 0);

void descriptor_set_texel_buffer_update(VkDevice device, VkDescriptorSet set, uint32_t binding, VkBufferView buffer_view, VkDescriptorType type,
                                        uint32_t array_element = 0);

void descriptor_set_batch_copy(VkDevice device, std::span<VkCopyDescriptorSet> descriptor_copies);

void descriptor_set_copy(VkDevice device, const VkCopyDescriptorSet* descriptor_copy);

void descriptor_set_batch_update(VkDevice device, std::span<VkWriteDescriptorSet> descriptor_writes,
                                 std::span<VkCopyDescriptorSet> descriptor_copies = {});

/*
 * CORE EXTENSIONS
 */

// VULKAN 1.3

void descriptor_set_inline_uniform_block_update(VkDevice device, VkDescriptorSet set, uint32_t binding, uint32_t data_size, const void* data,
                                                uint32_t array_element = 0);

/*
 * NON-CORE EXTENSIONS
 */

void descriptor_set_acceleration_structure_khr_update(VkDevice device, VkDescriptorSet set, uint32_t binding,
                                                      const VkAccelerationStructureKHR* acceleration_structure, uint32_t array_element = 0);
