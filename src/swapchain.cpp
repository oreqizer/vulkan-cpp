#include <vulkan/vulkan.h>
#include "swapchain.h"
#include "queue.h"

namespace {
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
        if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) {
            return {VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
        }

        for (const auto& availableFormat : availableFormats) {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return availableFormat;
            }
        }

        return availableFormats[0];
    }

    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes) {
        VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

        for (const auto& availablePresentMode : availablePresentModes) {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return availablePresentMode;
            } else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
                bestMode = availablePresentMode;
            }
        }

        return bestMode;
    }

    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, uint32_t width, uint32_t height) {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
            return capabilities.currentExtent;
        } else {
            VkExtent2D actualExtent = {width, height};

            actualExtent.width = std::max(
                    capabilities.minImageExtent.width,
                    std::min(capabilities.maxImageExtent.width, actualExtent.width)
            );
            actualExtent.height = std::max(
                    capabilities.minImageExtent.height,
                    std::min(capabilities.maxImageExtent.height, actualExtent.height)
            );

            return actualExtent;
        }
    }
}

Swapchain::Swapchain(VkSurfaceKHR surface, Device& device, uint32_t width, uint32_t height): device_(device) {
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device.getPhysical(), surface, &capabilities_);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device.getPhysical(), surface, &formatCount, nullptr);
    if (formatCount != 0) {
        formats_.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device.getPhysical(), surface, &formatCount, formats_.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device.getPhysical(), surface, &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        presentModes_.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device.getPhysical(), surface, &presentModeCount, presentModes_.data());
    }

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(formats_);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(presentModes_);
    extent_ = chooseSwapExtent(capabilities_, width, height);
    format_ = surfaceFormat.format;

    uint32_t imageCount = capabilities_.minImageCount + 1;
    if (capabilities_.maxImageCount > 0 && imageCount > capabilities_.maxImageCount) {
        imageCount = capabilities_.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo = {
            .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            .surface = surface,
            .minImageCount = imageCount,
            .imageFormat = surfaceFormat.format,
            .imageColorSpace = surfaceFormat.colorSpace,
            .imageExtent = extent_,
            .imageArrayLayers = 1,
            .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .preTransform = capabilities_.currentTransform,
            .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
            .presentMode = presentMode,
            .clipped = VK_TRUE,
            .oldSwapchain = VK_NULL_HANDLE,
    };

    Queue queue(surface, device.getPhysical());
    uint32_t queueFamilyIndices[] = {queue.getGraphicsFamily(), queue.getPresentFamily()};

    if (queue.getGraphicsFamily() != queue.getPresentFamily()) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices; // TODO deal with this
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0; // optional
        createInfo.pQueueFamilyIndices = nullptr; // optional
    }

    if (vkCreateSwapchainKHR(device.getLogical(), &createInfo, nullptr, &swapchain_) != VK_SUCCESS) {
        throw std::runtime_error("failed to create swap chain!");
    }

    vkGetSwapchainImagesKHR(device.getLogical(), swapchain_, &imageCount, nullptr);
    images_.resize(imageCount);
    imageViews_.resize(imageCount);
    vkGetSwapchainImagesKHR(device.getLogical(), swapchain_, &imageCount, images_.data());

    uint32_t i = 0;
    for (auto image : images_) {
        VkImageViewCreateInfo imageViewInfo = {
                .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                .image = image,
                .viewType = VK_IMAGE_VIEW_TYPE_2D,
                .format = format_,
                .components.r = VK_COMPONENT_SWIZZLE_IDENTITY,
                .components.g = VK_COMPONENT_SWIZZLE_IDENTITY,
                .components.b = VK_COMPONENT_SWIZZLE_IDENTITY,
                .components.a = VK_COMPONENT_SWIZZLE_IDENTITY,
                .subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .subresourceRange.baseMipLevel = 0,
                .subresourceRange.levelCount = 1,
                .subresourceRange.baseArrayLayer = 0,
                .subresourceRange.layerCount = 1,
        };

        if (vkCreateImageView(device.getLogical(), &imageViewInfo, nullptr, &imageViews_[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create image views!");
        }

        i++;
    }
}

Swapchain::~Swapchain() {
    for (auto imageView : imageViews_) {
        vkDestroyImageView(device_.getLogical(), imageView, nullptr);
    }

    vkDestroySwapchainKHR(device_.getLogical(), swapchain_, nullptr);
}

const bool Swapchain::isAdequate(VkSurfaceKHR surface, VkPhysicalDevice device) {
    uint32_t formatCount;
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

    return formatCount > 0 && presentModeCount > 0;
}
