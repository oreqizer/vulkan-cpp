#include <limits>

#include "frame.h"

void frame::draw(
        VkDevice device,
        VkQueue graphicsQueue,
        VkSwapchainKHR swapchain,
        std::vector<VkCommandBuffer> buffers,
        VkSemaphore semaphoreImageAvailable,
        VkSemaphore semaphoreRenderFinished
) {
    uint32_t imageIndex;
    vkAcquireNextImageKHR(
            device,
            swapchain,
            std::numeric_limits<uint64_t>::max(), // timeout, uint64_t max means none
            semaphoreImageAvailable, // sync semaphore
            VK_NULL_HANDLE, // fence, none since we're using semaphores
            &imageIndex
    );

    VkSemaphore waitSemaphores[] = {semaphoreImageAvailable};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    VkSemaphore signalSemaphores[] = {semaphoreRenderFinished};
    VkSubmitInfo submitInfo = {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = waitSemaphores,
            .pWaitDstStageMask = waitStages,
            .commandBufferCount = 1,
            .pCommandBuffers = &buffers[imageIndex],
            .signalSemaphoreCount = 1,
            .pSignalSemaphores = signalSemaphores,
    };

    // last parameter is a fence
    if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }
}
