#ifndef VULKAN_CPP_FRAME_H
#define VULKAN_CPP_FRAME_H

#include <vector>
#include <vulkan/vulkan.h>

#include "device.h"
#include "swapchain.h"

namespace frame {
    VkResult draw(
            Device& device,
            Swapchain& swapchain,
            std::vector<VkCommandBuffer> buffers,
            VkSemaphore semaphoreImageAvailable,
            VkSemaphore semaphoreRenderFinished
    );
}

#endif //VULKAN_CPP_FRAME_H
