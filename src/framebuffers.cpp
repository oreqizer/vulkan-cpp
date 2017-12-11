#include "framebuffers.h"

std::vector<VkFramebuffer> framebuffers::create(
        VkDevice device,
        VkExtent2D extent,
        std::vector<VkImageView> views,
        VkRenderPass renderPass
) {
    std::vector<VkFramebuffer> framebuffers(views.size());

    uint32_t i = 0;
    for (auto* view : views) {
        VkImageView attachments[] = {view};

        VkFramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments; // maybe fix escaping local scope
        framebufferInfo.width = extent.width;
        framebufferInfo.height = extent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &framebuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer!");
        }

        i++;
    }

    return framebuffers;
}

void framebuffers::destroy(VkDevice device, std::vector<VkFramebuffer> framebuffers) {
    for (auto framebuffer : framebuffers) {
        vkDestroyFramebuffer(device, framebuffer, nullptr);
    }
}
