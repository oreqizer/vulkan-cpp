#ifndef VULKAN_CPP_DEBUG_H
#define VULKAN_CPP_DEBUG_H

// MoltenVK does not support validation layers
#if defined(NDEBUG) || defined(__APPLE__)
    const bool DEBUG = false;
#else
    const bool DEBUG = true;
#endif

#ifdef NDEBUG
    const bool VERBOSE = false;
#else
    const bool VERBOSE = true;
#endif

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
