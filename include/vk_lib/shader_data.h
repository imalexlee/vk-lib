/*
 * Utilities regarding shader data binding with descriptors, push constants, and specialization constants
 */

#pragma once
#include <vk_lib/common.h>

namespace vk_lib {

[[nodiscard]] VkPushConstantRange push_constant_range(VkShaderStageFlags shader_stage_flags, uint32_t size, uint32_t offset = 0);

[[nodiscard]] VkSpecializationMapEntry specialization_map_entry(uint32_t constant_id, size_t size, uint32_t offset = 0);

[[nodiscard]] VkSpecializationInfo specialization_info(const void* data, uint32_t data_size, std::span<VkSpecializationMapEntry> map_entries);

[[nodiscard]] VkDescriptorSetLayoutBinding descriptor_set_layout_binding(uint32_t binding, VkDescriptorType type, uint32_t descriptor_count = 1,
                                                                         VkShaderStageFlags stages            = VK_SHADER_STAGE_ALL,
                                                                         const VkSampler*   immutable_sampler = nullptr);

[[nodiscard]] VkDescriptorSetLayoutCreateInfo descriptor_set_layout_create_info(std::span<VkDescriptorSetLayoutBinding> layout_bindings,
                                                                                VkDescriptorSetLayoutCreateFlags        flags = 0,
                                                                                const void*                             pNext = nullptr);

[[nodiscard]] VkDescriptorPoolSize descriptor_pool_size(VkDescriptorType type, uint32_t descriptor_count);

[[nodiscard]] VkDescriptorPoolCreateInfo descriptor_pool_create_info(uint32_t max_sets, std::span<VkDescriptorPoolSize> pool_sizes,
                                                                     VkDescriptorPoolCreateFlags flags = 0, const void* pNext = nullptr);

[[nodiscard]] VkDescriptorSetAllocateInfo descriptor_set_allocate_info(const VkDescriptorSetLayout* set_layout, VkDescriptorPool descriptor_pool,
                                                                       uint32_t descriptor_set_count = 1, const void* pNext = nullptr);

[[nodiscard]] VkCopyDescriptorSet copy_descriptor_set_create(VkDescriptorSet src_set, VkDescriptorSet dst_set, uint32_t src_binding,
                                                             uint32_t dst_binding, uint32_t src_array_element = 0, uint32_t dst_array_element = 0,
                                                             uint32_t descriptor_count = 1);

[[nodiscard]] VkDescriptorImageInfo descriptor_image_info(VkImageView image_view, VkImageLayout image_layout, VkSampler sampler = nullptr);

[[nodiscard]] VkDescriptorBufferInfo descriptor_buffer_info(VkBuffer buffer, uint64_t offset = 0, uint64_t range = VK_WHOLE_SIZE);

[[nodiscard]] VkWriteDescriptorSet write_descriptor_set(uint32_t binding, VkDescriptorType type, VkDescriptorSet descriptor_set,
                                                        const VkDescriptorImageInfo* image_info, const VkDescriptorBufferInfo* buffer_info = nullptr,
                                                        const VkBufferView* texel_buffer_view = nullptr, uint32_t array_element = 0,
                                                        uint32_t descriptor_count = 1, const void* pNext = nullptr);

/*
 * CORE EXTENSIONS
 */

// VULKAN 1.3

[[nodiscard]] VkWriteDescriptorSetInlineUniformBlockEXT write_descriptor_set_inline_uniform_block(uint32_t data_size, const void* data,
                                                                                                  const void* pNext = nullptr);

/*
 * NON-CORE EXTENSIONS
 */

[[nodiscard]] VkWriteDescriptorSetAccelerationStructureKHR
write_descriptor_set_acceleration_structure_khr_batch(std::span<VkAccelerationStructureKHR> acceleration_structures, const void* pNext = nullptr);

[[nodiscard]] VkWriteDescriptorSetAccelerationStructureKHR
write_descriptor_set_acceleration_structure_khr(const VkAccelerationStructureKHR* acceleration_structure, const void* pNext = nullptr);

} // namespace vk_lib