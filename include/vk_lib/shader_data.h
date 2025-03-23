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

VkDescriptorPoolSize descriptor_pool_size_create(VkDescriptorType type, uint32_t descriptor_count);

VkResult descriptor_pool_create(VkDevice device, uint32_t max_descriptor_sets, std::span<VkDescriptorPoolSize> descriptor_pool_sizes,
                                VkDescriptorPool* descriptor_pool, VkDescriptorPoolCreateFlags flags = 0, const void* pNext = nullptr);

VkResult descriptor_pool_reset(VkDevice device, VkDescriptorPool descriptor_pool);

void descriptor_pool_destroy(VkDevice device, VkDescriptorPool descriptor_pool);

VkResult descriptor_set_allocate(VkDevice device, VkDescriptorPool descriptor_pool, VkDescriptorSetLayout descriptor_set_layout,
                                 VkDescriptorSet* descriptor_set, const void* pNext = nullptr);