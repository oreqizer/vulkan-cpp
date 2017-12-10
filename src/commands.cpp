#include "commands.h"
#include "queue.h"

VkCommandPool commands::createPool(VkSurfaceKHR surface, VkPhysicalDevice physicalDevice, VkDevice device) {
    // TODO turn the following to a class - this is in the code 3 times minimum already
    queue::FamilyIndices queueFamilyIndices = queue::findFamilies(surface, physicalDevice);

    VkCommandPoolCreateInfo poolInfo = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .queueFamilyIndex = static_cast<uint32_t>(queueFamilyIndices.graphicsFamily),
            .flags = 0, // optional
    };

    VkCommandPool pool;
    if (vkCreateCommandPool(device, &poolInfo, nullptr, &pool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create command pool!");
    }

    return pool;
}

void commands::destroyPool(VkDevice device, VkCommandPool pool) {
    vkDestroyCommandPool(device, pool, nullptr);
}

std::vector<VkCommandBuffer> commands::createBuffers(
        VkDevice device,
        VkExtent2D extent,
        VkRenderPass renderPass,
        VkPipeline pipeline,
        std::vector<VkFramebuffer> framebuffers,
        VkCommandPool pool,
        uint64_t count
) {
    std::vector<VkCommandBuffer> buffers(count);

    VkCommandBufferAllocateInfo allocInfo = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .commandPool = pool,
            .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = static_cast<uint32_t>(count),
    };

    if (vkAllocateCommandBuffers(device, &allocInfo, buffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }

    uint32_t i = 0;
    for (auto* buffer : buffers) {
        VkCommandBufferBeginInfo beginInfo = {
                .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
                .flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT,
        };

        vkBeginCommandBuffer(buffer, &beginInfo);

        VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
        VkRenderPassBeginInfo renderPassInfo = {
                .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
                .renderPass = renderPass,
                .framebuffer = framebuffers[i],
                .renderArea.offset = {0, 0},
                .renderArea.extent = extent,
                .clearValueCount = 1,
                .pClearValues = &clearColor,
        };

        vkCmdBeginRenderPass(buffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
        vkCmdDraw(buffer, 3, 1, 0, 0);
        vkCmdEndRenderPass(buffer);

        if (vkEndCommandBuffer(buffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }

        i++;
    }

    return buffers;
}
