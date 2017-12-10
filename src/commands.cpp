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

std::vector<VkCommandBuffer> commands::createBuffers(VkDevice device, VkCommandPool pool, uint64_t count) {
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

    return buffers;
}
