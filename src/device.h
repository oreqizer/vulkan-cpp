#ifndef VULKAN_CPP_DEVICE_H
#define VULKAN_CPP_DEVICE_H

#include <vulkan/vulkan.h>

#include "queue.h"

class Device {
public:
    Device(VkInstance instance, VkSurfaceKHR surface);
    ~Device();

    static const bool isAdequate(VkPhysicalDevice device);

    uint32_t findMemoryType(uint32_t filter, VkMemoryPropertyFlags flags) const;

    const VkPhysicalDevice getPhysical() const { return physical_; }
    const VkDevice getLogical() const { return logical_; }
    const Queue& getQueue() const { return *queue_; }
    const VkQueue getGraphicsQueue() const { return graphicsQueue_; }
    const VkQueue getPresentQueue() const { return presentQueue_; }
private:
    VkPhysicalDevice physical_;
    VkDevice logical_;
    Queue* queue_;
    VkQueue graphicsQueue_;
    VkQueue presentQueue_;
};

#endif //VULKAN_CPP_DEVICE_H
