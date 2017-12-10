#ifndef VULKAN_CPP_FRAMEBUFFERS_H
#define VULKAN_CPP_FRAMEBUFFERS_H

#include <vector>
#include <vulkan/vulkan.h>

namespace framebuffers {
    std::vector<VkFramebuffer> create(VkDevice device);
    void destroy(VkDevice device, std::vector<VkFramebuffer> framebuffers);
}

#endif //VULKAN_CPP_FRAMEBUFFERS_H
