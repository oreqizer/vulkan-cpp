#ifndef VULKAN_CPP_BUFFER_H
#define VULKAN_CPP_BUFFER_H

#include <vulkan/vulkan.h>

class Buffer {
public:
    Buffer(
            VkPhysicalDevice physicalDevice,
            VkDevice device,
            VkDeviceSize size,
            VkBufferUsageFlags usage,
            VkMemoryPropertyFlags properties
    );
    ~Buffer();

    inline const auto getBuffer() {
        return buffer_;
    }

    inline const auto getMemory() {
        return memory_;
    }

private:
    VkDevice device_;
    VkBuffer buffer_;
    VkDeviceMemory memory_;
};

#endif //VULKAN_CPP_BUFFER_H
