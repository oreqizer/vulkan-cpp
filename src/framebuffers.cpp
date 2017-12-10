#include "framebuffers.h"

std::vector<VkFramebuffer> framebuffers::create(VkDevice device) {

}

void framebuffers::destroy(VkDevice device, std::vector<VkFramebuffer> framebuffers) {
    for (auto framebuffer : framebuffers) {
        vkDestroyFramebuffer(device, framebuffer, nullptr);
    }
}
