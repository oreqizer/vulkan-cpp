#ifndef VULKAN_CPP_PIPELINE_H
#define VULKAN_CPP_PIPELINE_H

#include <vulkan/vulkan.h>

namespace pipeline {
    VkPipelineLayout createLayout(VkDevice device, VkExtent2D extent);
    void destroy(VkPipelineLayout layout);
}

#endif //VULKAN_CPP_PIPELINE_H
