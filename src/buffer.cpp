#include <stdexcept>

#include "buffer.h"

Buffer::Buffer(
        Device& device,
        VkDeviceSize size,
        VkBufferUsageFlags usage,
        VkMemoryPropertyFlags properties
): device_(device) {
    VkBufferCreateInfo bufferInfo = {
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .size = size,
            .usage = usage,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    };

    VkBuffer buffer;
    if (vkCreateBuffer(device.getLogical(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create vertex buffer!");
    }
    VkMemoryRequirements requirements = {};
    vkGetBufferMemoryRequirements(device.getLogical(), buffer, &requirements);

    VkMemoryAllocateInfo allocInfo = {
            .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .allocationSize = requirements.size,
            .memoryTypeIndex = device.findMemoryType(requirements.memoryTypeBits, properties),
    };

    VkDeviceMemory memory;
    if (vkAllocateMemory(device.getLogical(), &allocInfo, nullptr, &memory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate vertex buffer memory!");
    }

    vkBindBufferMemory(device.getLogical(), buffer, memory, 0);

    buffer_ = buffer;
    memory_ = memory;
}

Buffer::~Buffer() {
    vkDestroyBuffer(device_.getLogical(), buffer_, nullptr);
    vkFreeMemory(device_.getLogical(), memory_, nullptr);
}
