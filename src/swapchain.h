#ifndef VULKAN_CPP_SWAP_CHAIN_H
#define VULKAN_CPP_SWAP_CHAIN_H

#include <vector>

namespace swapchain {
    struct Data {
        VkSwapchainKHR instance;
        std::vector<VkImage> images;
        VkFormat format;
        VkExtent2D extent;
    };

    struct SupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    Data setup(
            VkSurfaceKHR surface,
            VkPhysicalDevice physicalDevice,
            VkDevice device,
            uint32_t width,
            uint32_t height
    );
    void destroy(VkDevice device, VkSwapchainKHR swapchain);

    SupportDetails querySupport(VkSurfaceKHR surface, VkPhysicalDevice device);
}

#endif //VULKAN_CPP_SWAP_CHAIN_H
