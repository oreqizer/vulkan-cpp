#include <limits>
#include <iostream>

#include "frame.h"

VkResult frame::draw(
        Device& device,
        Swapchain& swapchain,
        std::vector<VkCommandBuffer> buffers,
        VkSemaphore semaphoreImageAvailable,
        VkSemaphore semaphoreRenderFinished
) {
    uint32_t imageIndex;
    auto result = vkAcquireNextImageKHR(
            device.getLogical(),
            swapchain.getSwapchain(),
            std::numeric_limits<uint64_t>::max(), // timeout, uint64_t max means none
            semaphoreImageAvailable, // sync semaphore
            VK_NULL_HANDLE, // fence, none since we're using semaphores
            &imageIndex
    );

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        return VK_ERROR_OUT_OF_DATE_KHR;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

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
    if (vkQueueSubmit(device.getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkSwapchainKHR swapChains[] = {swapchain.getSwapchain()};
    VkPresentInfoKHR presentInfo = {
            .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = signalSemaphores,
            .swapchainCount = 1,
            .pSwapchains = swapChains,
            .pImageIndices = &imageIndex,
            .pResults = nullptr, // optional
    };

    vkQueuePresentKHR(device.getPresentQueue(), &presentInfo);
    vkQueueWaitIdle(device.getPresentQueue()); // sync with the GPU

    return result;
}
