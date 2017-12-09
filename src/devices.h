#ifndef VULKAN_CPP_DEVICES_H
#define VULKAN_CPP_DEVICES_H

#include <vulkan/vulkan.h>

namespace devices {
    const std::vector<const char*> EXTENSIONS = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    };

    VkPhysicalDevice pickPhysical(VkInstance instance, VkSurfaceKHR surface);

    VkDevice setupLogical(
            VkSurfaceKHR surface,
            VkPhysicalDevice physicalDevice,
            VkQueue graphicsQueue,
            VkQueue presentQueue
    );
    void destroyLogical(VkDevice device);
}

#endif //VULKAN_CPP_DEVICES_H
