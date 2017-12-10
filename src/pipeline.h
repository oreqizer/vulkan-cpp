#ifndef VULKAN_CPP_PIPELINE_H
#define VULKAN_CPP_PIPELINE_H

#include <vulkan/vulkan.h>

namespace pipeline {
    struct Data {
        VkPipelineLayout layout;
        VkPipeline instance;
    };

    VkRenderPass createRenderPass(VkDevice device, VkFormat format);
    void destroyRenderPass(VkDevice device, VkRenderPass renderPass);
    Data create(VkDevice device, VkExtent2D extent, VkRenderPass renderPass);
    void destroy(VkDevice device, VkPipelineLayout layout, VkPipeline pipeline);
}

#endif //VULKAN_CPP_PIPELINE_H
