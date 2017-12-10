#ifndef VULKAN_CPP_FRAMEBUFFERS_H
#define VULKAN_CPP_FRAMEBUFFERS_H

#include <vector>
#include <vulkan/vulkan.h>

namespace framebuffers {
    std::vector<VkFramebuffer> create(
            VkDevice device,
            VkExtent2D extent,
            std::vector<VkImageView> views,
            VkRenderPass renderPass
    );
    void destroy(VkDevice device, std::vector<VkFramebuffer> framebuffers);
}

#endif //VULKAN_CPP_FRAMEBUFFERS_H
