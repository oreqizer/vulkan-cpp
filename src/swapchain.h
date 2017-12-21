#ifndef VULKAN_CPP_SWAP_CHAIN_H
#define VULKAN_CPP_SWAP_CHAIN_H

#include <vector>

#include "device.h"

class Swapchain {
public:
    Swapchain(VkSurfaceKHR surface, Device& device, uint32_t width, uint32_t height);
    ~Swapchain();

    static const bool isAdequate(VkSurfaceKHR surface, VkPhysicalDevice device);

    const VkSwapchainKHR& getSwapchain() const { return swapchain_; }
    const VkFormat& getFormat() const { return format_; }
    const VkExtent2D& getExtent() const { return extent_; }
    const VkSurfaceCapabilitiesKHR& getCapabilities() const { return capabilities_; }
    const std::vector<VkSurfaceFormatKHR>& getFormats() const { return formats_; }
    const std::vector<VkPresentModeKHR>& getPresentModes() const { return presentModes_; }
    const std::vector<VkImage>& getImages() const { return images_; }
    const std::vector<VkImageView>& getImageViews() const { return imageViews_; }

private:
    Device& device_;
    VkSwapchainKHR swapchain_;
    VkFormat format_;
    VkExtent2D extent_;
    VkSurfaceCapabilitiesKHR capabilities_;
    std::vector<VkSurfaceFormatKHR> formats_;
    std::vector<VkPresentModeKHR> presentModes_;
    std::vector<VkImage> images_;
    std::vector<VkImageView> imageViews_;
};

#endif //VULKAN_CPP_SWAP_CHAIN_H
