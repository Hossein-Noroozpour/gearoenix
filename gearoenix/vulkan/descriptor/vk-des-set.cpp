#include "vk-des-set.hpp"
#include "../../core/cr-static.hpp"
#include "../../render/shader/rnd-shd-shader.hpp"
#include "../device/vk-dev-logical.hpp"
#include "../device/vk-dev-physical.hpp"
#include "../pipeline/vk-pip-layout.hpp"
#include "../vk-check.hpp"
#include "../vk-instance.hpp"
#include "vk-des-pool.hpp"
#include "vk-des-set-layout.hpp"

gearoenix::render::descriptor::Set::Set(
    Pool* pool,
    SetLayout* lay,
    const VkDescriptorBufferInfo& buff_info)
    : pool(pool)
{
    const device::Logical* d = pool->get_logical_device();
    const VkDevice vkdev = d->get_vulkan_data();
    const Linker* l = d->get_physical_device()->get_instance()->get_linker();
    VkDescriptorSetAllocateInfo alloc_info;
    setz(alloc_info);
    alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    alloc_info.descriptorPool = pool->get_vulkan_data();
    alloc_info.descriptorSetCount = 1;
    alloc_info.pSetLayouts = &(lay->get_vulkan_data());
    VKC(l->vkAllocateDescriptorSets(vkdev, &alloc_info, &vulkan_data));
    VkWriteDescriptorSet write_descriptor_set;
    setz(write_descriptor_set);
    write_descriptor_set.descriptorCount = 1;
    write_descriptor_set.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    write_descriptor_set.dstBinding = 0;
    write_descriptor_set.dstSet = vulkan_data;
    write_descriptor_set.pBufferInfo = &buff_info;
    write_descriptor_set.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    l->vkUpdateDescriptorSets(vkdev, 1, &write_descriptor_set, 0, nullptr);
}

gearoenix::render::descriptor::Set::Set(Pool* pool,
    SetLayout* lay,
    const VkDescriptorBufferInfo& buff_info,
    const VkDescriptorImageInfo& img_info)
    : pool(pool)
{
    const device::Logical* d = pool->get_logical_device();
    const VkDevice vkdev = d->get_vulkan_data();
    const Linker* l = d->get_physical_device()->get_instance()->get_linker();
    VkDescriptorSetAllocateInfo alloc_info;
    setz(alloc_info);
    alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    alloc_info.descriptorPool = pool->get_vulkan_data();
    alloc_info.descriptorSetCount = 1;
    alloc_info.pSetLayouts = &(lay->get_vulkan_data());
    VKC(l->vkAllocateDescriptorSets(vkdev, &alloc_info, &vulkan_data));
    VkWriteDescriptorSet write_descriptor_sets[2];
    setz(write_descriptor_sets[0]);
    write_descriptor_sets[0].descriptorCount = 1;
    write_descriptor_sets[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    write_descriptor_sets[0].dstBinding = 0;
    write_descriptor_sets[0].dstSet = vulkan_data;
    write_descriptor_sets[0].pBufferInfo = &buff_info;
    write_descriptor_sets[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    setz(write_descriptor_sets[1]);
    write_descriptor_sets[1].descriptorCount = 1;
    write_descriptor_sets[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    write_descriptor_sets[1].dstBinding = 1;
    write_descriptor_sets[1].dstSet = vulkan_data;
    write_descriptor_sets[1].pImageInfo = &img_info;
    write_descriptor_sets[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    l->vkUpdateDescriptorSets(vkdev, countof(write_descriptor_sets), write_descriptor_sets, 0, nullptr);
}

gearoenix::render::descriptor::Set::~Set() {}

const VkDescriptorSet& gearoenix::render::descriptor::Set::get_vulkan_data() const
{
    return vulkan_data;
}

const VkDescriptorSetLayout& gearoenix::render::descriptor::Set::get_layout() const
{
    return layout;
}

const gearoenix::render::descriptor::Pool* gearoenix::render::descriptor::Set::get_pool() const
{
    return pool;
}

gearoenix::render::descriptor::Pool* gearoenix::render::descriptor::Set::get_pool()
{
    return pool;
}
