#ifndef VULKAN_CPP_IMAGES_H
#define VULKAN_CPP_IMAGES_H

#include <vulkan/vulkan.h>
#include <vector>

namespace views {
    std::vector<VkImageView> create(VkDevice device, std::vector<VkImage> images, VkFormat format);
    void destroy(VkDevice device, std::vector<VkImageView> views);
}

#endif //VULKAN_CPP_IMAGES_H
