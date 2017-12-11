#ifndef VULKAN_CPP_DEBUG_H
#define VULKAN_CPP_DEBUG_H

#ifdef __APPLE__
    const bool APPLE = true;
#else
    const bool APPLE = false;
#endif

#ifdef NDEBUG
    const bool DEBUG = false;
    const bool VERBOSE = false;
#else
    // MoltenVK does not support validation layers
    const bool DEBUG = !APPLE;
    const bool VERBOSE = true;
#endif

#include <vector>
#include <vulkan/vulkan.h>

namespace debug {
    const bool DEBUG = DEBUG;
    const bool VERBOSE = VERBOSE;
    const std::vector<const char*> VALIDATION_LAYERS = {
            "VK_LAYER_LUNARG_standard_validation",
    };

    bool checkValidationLayerSupport();
    VkDebugReportCallbackEXT setupCallback(VkInstance instance);
    void destroyCallback(VkInstance instance, VkDebugReportCallbackEXT callback);
};

#endif //VULKAN_CPP_DEBUG_H
