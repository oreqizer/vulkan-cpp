#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>

#include "instance.h"
#include "debug.h"

// anon namespace
// https://stackoverflow.com/a/16813558/4337281
namespace {
    std::vector<const char*> getRequiredExtensions() {
        std::vector<const char*> extensions;

        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        for (uint32_t i = 0; i < glfwExtensionCount; i++) {
            extensions.emplace_back(glfwExtensions[i]);
        }

        if (debug::DEBUG) {
            extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
        }

        return extensions;
    }
}

VkInstance instance::create() {
    VkInstance instance;

    if (debug::DEBUG && !debug::checkValidationLayerSupport()) {
        throw std::runtime_error("validation layers requested, but not available!");
    }

    VkApplicationInfo appInfo = {
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pApplicationName = "Hello Triangle",
            .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
            .pEngineName = "No Engine",
            .engineVersion = VK_MAKE_VERSION(1, 0, 0),
            .apiVersion = VK_API_VERSION_1_0,
    };

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    VkInstanceCreateInfo createInfo = {
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pApplicationInfo = &appInfo,
            .enabledExtensionCount = glfwExtensionCount,
            .ppEnabledExtensionNames = glfwExtensions,
            .enabledLayerCount = 0,
    };

    if (debug::DEBUG) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(debug::VALIDATION_LAYERS.size());
        createInfo.ppEnabledLayerNames = debug::VALIDATION_LAYERS.data();
    }

    auto extensions = getRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    VkResult result = vkCreateInstance(
            &createInfo,
            nullptr, // pointer to a custom allocator callback
            &instance
    );

    if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to create Vulkan instance");
    }

    // VK Extensions
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data());

    if (debug::VERBOSE) {
        std::cout << "available extensions:" << std::endl;
        for (const auto& extension : availableExtensions) {
            std::cout << "\t" << extension.extensionName << std::endl;
        }
    }

    return instance;
}

void instance::destroy(VkInstance instance) {
    vkDestroyInstance(instance, nullptr);
}
