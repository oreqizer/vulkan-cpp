#ifndef VULKAN_CPP_INSTANCE_H
#define VULKAN_CPP_INSTANCE_H


#include <vulkan/vulkan.h>

namespace instance {
    VkInstance create();
    void destroy(VkInstance instance);
}

#endif //VULKAN_CPP_INSTANCE_H
