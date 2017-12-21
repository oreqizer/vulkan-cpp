#include <vector>

#include "queue.h"

Queue::Queue(VkSurfaceKHR surface, VkPhysicalDevice device) {
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    uint32_t i = 0;
    for (const auto& queueFamily : queueFamilies) {
        auto presentSupport = static_cast<VkBool32>(false);
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
        if (queueFamily.queueCount > 0 && presentSupport) {
            presentFamily_ = i;
        }

        if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            graphicsFamily_ = i;
        }

        if (graphicsFamily_ != UINT32_MAX && presentFamily_ != UINT32_MAX) {
            break;
        }

        i++;
    }
}

const bool Queue::isComplete() const {
    return graphicsFamily_ != UINT32_MAX && presentFamily_ != UINT32_MAX;
}
