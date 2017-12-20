#ifndef VULKAN_CPP_DEVICE_H
#define VULKAN_CPP_DEVICE_H

#include <vulkan/vulkan.h>

class Device {
public:
    Device(VkInstance instance, VkSurfaceKHR surface);
    ~Device();

    uint32_t findMemoryType(uint32_t filter, VkMemoryPropertyFlags flags);

    inline const auto getPhysical() { return physicalDevice_; }
    inline const auto getLogical() { return logicalDevice_; }
    inline const auto getGraphicsQueue() { return graphicsQueue_; }
    inline const auto getPresentQueue() { return presentQueue_; }
private:
    VkPhysicalDevice physicalDevice_;
    VkDevice logicalDevice_;
    VkQueue graphicsQueue_;
    VkQueue presentQueue_;
};

#endif //VULKAN_CPP_DEVICE_H
