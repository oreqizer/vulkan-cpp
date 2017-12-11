#include "views.h"

std::vector<VkImageView> views::create(VkDevice device, std::vector<VkImage> images, VkFormat format) {
    std::vector<VkImageView> views(images.size());

    uint32_t i = 0;
    for (auto image : images) {
        VkImageViewCreateInfo createInfo = {
                .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                .image = image,
                .viewType = VK_IMAGE_VIEW_TYPE_2D,
                .format = format,
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

        if (vkCreateImageView(device, &createInfo, nullptr, &views[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create image views!");
        }

        i++;
    }

    return views;
}

void views::destroy(VkDevice device, std::vector<VkImageView> views) {
    for (auto imageView : views) {
        vkDestroyImageView(device, imageView, nullptr);
    }
}
