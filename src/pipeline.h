#ifndef VULKAN_CPP_PIPELINE_H
#define VULKAN_CPP_PIPELINE_H

#include <vulkan/vulkan.h>

namespace pipeline {
    VkRenderPass createRenderPass(VkDevice device, VkFormat format);
    void destroyRenderPass(VkDevice device, VkRenderPass renderPass);
    VkPipelineLayout createLayout(VkDevice device, VkExtent2D extent);
    void destroyLayout(VkDevice device, VkPipelineLayout layout);
}

#endif //VULKAN_CPP_PIPELINE_H
