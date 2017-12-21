#ifndef VULKAN_CPP_QUEUE_H
#define VULKAN_CPP_QUEUE_H

#include <vulkan/vulkan.h>

class Queue {
public:
    Queue(VkSurfaceKHR surface, VkPhysicalDevice device);

    const bool isComplete() const;

    const uint32_t getPresentFamily() const { return presentFamily_; }
    const uint32_t getGraphicsFamily() const { return graphicsFamily_; }
private:
    uint32_t presentFamily_ = UINT32_MAX;
    uint32_t graphicsFamily_ = UINT32_MAX;
};

#endif //VULKAN_CPP_QUEUE_H
