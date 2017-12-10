#include <stdexcept>

#include "semaphore.h"

VkSemaphore semaphore::create(VkDevice device) {
    VkSemaphoreCreateInfo semaphoreInfo = {
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
    };

    VkSemaphore semaphore;
    if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &semaphore) != VK_SUCCESS) {
        throw std::runtime_error("failed to create semaphores!");
    }

    return semaphore;
}

void semaphore::destroy(VkDevice device, VkSemaphore semaphore) {
    vkDestroySemaphore(device, semaphore, nullptr);
}
