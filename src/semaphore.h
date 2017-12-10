#ifndef VULKAN_CPP_SEMAPHORE_H
#define VULKAN_CPP_SEMAPHORE_H

#include <vulkan/vulkan.h>

namespace semaphore {
    VkSemaphore create(VkDevice device);
    void destroy(VkDevice device, VkSemaphore semaphore);
}

#endif //VULKAN_CPP_SEMAPHORE_H
