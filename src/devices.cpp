#include <stdexcept>
#include <vector>
#include <set>
#include <string>

#include "devices.h"
#include "debug.h"
#include "queue.h"
#include "swapchain.h"

namespace {
    bool checkDeviceExtensionSupport(VkPhysicalDevice device) {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(devices::EXTENSIONS.begin(), devices::EXTENSIONS.end());

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

        bool swapChainAdequate = false;
        if (extensionsSupported) {
            swapchain::SupportDetails swapChainSupport = swapchain::querySupport(surface, device);
            swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
        }

        return indices.isComplete() && extensionsSupported && swapChainAdequate;
    }
}

VkPhysicalDevice devices::pickPhysical(VkInstance instance, VkSurfaceKHR surface) {
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

    return physicalDevice;
}

devices::Data devices::createLogical(VkSurfaceKHR surface, VkPhysicalDevice physicalDevice)  {
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
            .enabledExtensionCount = static_cast<uint32_t>(devices::EXTENSIONS.size()),
            .ppEnabledExtensionNames = devices::EXTENSIONS.data(),
    };

    if (debug::DEBUG) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(debug::VALIDATION_LAYERS.size());
        createInfo.ppEnabledLayerNames = debug::VALIDATION_LAYERS.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    devices::Data data = {};
    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &data.device) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }

    vkGetDeviceQueue(data.device, static_cast<uint32_t>(indices.graphicsFamily), 0, &data.graphicsQueue);
    vkGetDeviceQueue(data.device, static_cast<uint32_t>(indices.presentFamily), 0, &data.presentQueue);

    return data;
}

void devices::destroyLogical(VkDevice device) {
    vkDestroyDevice(device, nullptr);
}

uint32_t devices::findMemoryType(VkPhysicalDevice physicalDevice, uint32_t filter, VkMemoryPropertyFlags flags) {
    VkPhysicalDeviceMemoryProperties properties = {};
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &properties);

    for (uint32_t i = 0; i < properties.memoryTypeCount; i++) {
        if ((filter & (1 << i)) && (properties.memoryTypes[i].propertyFlags & flags) == flags) {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}