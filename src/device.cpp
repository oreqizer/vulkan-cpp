#include <stdexcept>
#include <vector>
#include <set>
#include <string>

#include "device.h"
#include "queue.h"
#include "swapchain.h"
#include "debug.h"

namespace {
    const std::vector<const char*> EXTENSIONS = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    };

    bool checkDeviceExtensionSupport(VkPhysicalDevice device) {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(EXTENSIONS.begin(), EXTENSIONS.end());

        for (const auto& extension : availableExtensions) {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }

    bool isDeviceSuitable(VkSurfaceKHR surface, VkPhysicalDevice device) {
        // normally would also do a feature check here
        // this checks just queue families
        queue::FamilyIndices indices = queue::findFamilies(surface, device);
        bool extensionsSupported = checkDeviceExtensionSupport(device);

        bool swapchainAdequate = false;
        if (extensionsSupported) {
            swapchain::SupportDetails swapChainSupport = swapchain::querySupport(surface, device);
            swapchainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
        }

        return indices.isComplete() && extensionsSupported && swapchainAdequate;
    }
}

Device::Device(VkInstance instance, VkSurfaceKHR surface) {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    if (deviceCount == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    for (const auto& device : devices) {
        if (isDeviceSuitable(surface, device)) {
            physicalDevice = device;
            break;
        }
    }

    if (physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("failed to find a suitable GPU!");
    }

    queue::FamilyIndices indices = queue::findFamilies(surface, physicalDevice);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {
            static_cast<uint32_t>(indices.graphicsFamily),
            static_cast<uint32_t>(indices.presentFamily),
    };

    float queuePriority = 1.0f;
    for (auto queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo = {
                .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                .queueFamilyIndex = queueFamily,
                .queueCount = 1,
                .pQueuePriorities = &queuePriority,
        };

        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures = {};

    VkDeviceCreateInfo createInfo = {
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .pQueueCreateInfos = queueCreateInfos.data(),
            .queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size()),
            .pEnabledFeatures = &deviceFeatures,
            .enabledExtensionCount = static_cast<uint32_t>(EXTENSIONS.size()),
            .ppEnabledExtensionNames = EXTENSIONS.data(),
    };

    if (debug::DEBUG) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(debug::VALIDATION_LAYERS.size());
        createInfo.ppEnabledLayerNames = debug::VALIDATION_LAYERS.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &logicalDevice_) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }

    vkGetDeviceQueue(logicalDevice_, static_cast<uint32_t>(indices.graphicsFamily), 0, &graphicsQueue_);
    vkGetDeviceQueue(logicalDevice_, static_cast<uint32_t>(indices.presentFamily), 0, &presentQueue_);

    physicalDevice_ = physicalDevice;
}

Device::~Device() {
    vkDestroyDevice(logicalDevice_, nullptr);
}

uint32_t Device::findMemoryType(uint32_t filter, VkMemoryPropertyFlags flags) {
    VkPhysicalDeviceMemoryProperties properties = {};
    vkGetPhysicalDeviceMemoryProperties(physicalDevice_, &properties);

    for (uint32_t i = 0; i < properties.memoryTypeCount; i++) {
        if ((filter & (1 << i)) && (properties.memoryTypes[i].propertyFlags & flags) == flags) {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}
