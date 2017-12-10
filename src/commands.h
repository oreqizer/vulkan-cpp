#ifndef VULKAN_CPP_COMMANDS_H
#define VULKAN_CPP_COMMANDS_H

#include <vector>
#include <vulkan/vulkan.h>

namespace commands {
    VkCommandPool createPool(VkSurfaceKHR surface, VkPhysicalDevice physicalDevice, VkDevice device);
    void destroyPool(VkDevice device, VkCommandPool pool);
    std::vector<VkCommandBuffer> createBuffers(VkDevice device);
    void destroyBuffers(VkDevice device, std::vector<VkCommandBuffer> buffers);
}

#endif //VULKAN_CPP_COMMANDS_H
