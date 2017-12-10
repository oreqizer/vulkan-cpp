#ifndef VULKAN_CPP_FRAME_H
#define VULKAN_CPP_FRAME_H

#include <vector>
#include <vulkan/vulkan.h>

namespace frame {
    void draw(
            VkDevice device,
            VkQueue graphicsQueue,
            VkQueue presentQueue,
            VkSwapchainKHR swapchain,
            std::vector<VkCommandBuffer> buffers,
            VkSemaphore semaphoreImageAvailable,
            VkSemaphore semaphoreRenderFinished
    );
}

#endif //VULKAN_CPP_FRAME_H
