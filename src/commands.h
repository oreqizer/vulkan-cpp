#ifndef VULKAN_CPP_COMMANDS_H
#define VULKAN_CPP_COMMANDS_H

#include <vector>
#include <vulkan/vulkan.h>

namespace commands {
    VkCommandPool createPool(VkSurfaceKHR surface, VkPhysicalDevice physicalDevice, VkDevice device);
    void destroyPool(VkDevice device, VkCommandPool pool);
    std::vector<VkCommandBuffer> createBuffers(
            VkDevice device,
            VkExtent2D extent,
            VkRenderPass renderPass,
            VkPipeline pipeline,
            std::vector<VkFramebuffer> framebuffers,
            VkCommandPool pool,
            uint64_t count
    );
    void destroyBuffers(VkDevice device, VkCommandPool pool, std::vector<VkCommandBuffer> buffers);
}

#endif //VULKAN_CPP_COMMANDS_H
