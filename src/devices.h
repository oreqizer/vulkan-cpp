#ifndef VULKAN_CPP_DEVICES_H
#define VULKAN_CPP_DEVICES_H

#include <vulkan/vulkan.h>

namespace devices {
    const std::vector<const char*> EXTENSIONS = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    };

    struct Data {
        VkDevice device;
        VkQueue graphicsQueue;
        VkQueue presentQueue;
    };

    VkPhysicalDevice pickPhysical(VkInstance instance, VkSurfaceKHR surface);
    Data createLogical(VkSurfaceKHR surface, VkPhysicalDevice physicalDevice);
    void destroyLogical(VkDevice device);
    uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t filter, VkMemoryPropertyFlags flags);
}

#endif //VULKAN_CPP_DEVICES_H
