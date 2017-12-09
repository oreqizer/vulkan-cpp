#ifndef VULKAN_CPP_QUEUE_H
#define VULKAN_CPP_QUEUE_H

#include <vulkan/vulkan.h>

namespace queue {
    class FamilyIndices {
    public:
        int graphicsFamily = -1;
        int presentFamily = -1;

        bool isComplete() {
            return graphicsFamily >= 0 && presentFamily >= 0;
        }
    };

    FamilyIndices findFamilies(VkSurfaceKHR surface, VkPhysicalDevice device);
}

#endif //VULKAN_CPP_QUEUE_H
