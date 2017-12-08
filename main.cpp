#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include <iostream>
#include <vector>
#include <stdexcept>
#include <functional>

const int WIDTH = 800;
const int HEIGHT = 600;

// MoltenVK does not support validation layers
#if defined(NDEBUG) || defined(__APPLE__)
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

// DEBUG
VkResult createDebugReportCallbackEXT(
        VkInstance instance,
        const VkDebugReportCallbackCreateInfoEXT* pCreateInfo,
        const VkAllocationCallbacks* pAllocator,
        VkDebugReportCallbackEXT* pCallback
) {
    auto func = (PFN_vkCreateDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pCallback);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

// DEBUG
void destroyDebugReportCallbackEXT(
        VkInstance instance,
        VkDebugReportCallbackEXT callback,
        const VkAllocationCallbacks* pAllocator
) {
    auto func = (PFN_vkDestroyDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
    if (func != nullptr) {
        func(instance, callback, pAllocator);
    }
}

class HelloTriangleApplication {
public:
    void Run() {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

private:
    GLFWwindow* m_window;
    VkInstance m_instance;
    VkDebugReportCallbackEXT m_callback;

    const std::vector<const char*> m_validationLayers = {
            "VK_LAYER_LUNARG_standard_validation"
    };

    void initWindow() {
        glfwInit();

        // tells GLFW not to enable OpenGL context
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        // no need to resize window for now
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        m_window = glfwCreateWindow(
                WIDTH,
                HEIGHT,
                "Vulkan",
                nullptr, // monitor to open the window on
                nullptr  // something for OpenGL
        );
    }

    void initVulkan() {
        createInstance();
        setupDebugCallback();
    }

    void mainLoop() {
        while (!glfwWindowShouldClose(m_window)) {
            glfwPollEvents();
        }
    }

    void cleanup() {
        destroyDebugReportCallbackEXT(m_instance, m_callback, nullptr);
        vkDestroyInstance(m_instance, nullptr); // 2nd is a custom deallocator
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    void createInstance() {
        // DEBUG
         if (enableValidationLayers && !checkValidationLayerSupport()) {
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

        // DEBUG
        if (enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(m_validationLayers.size());
            createInfo.ppEnabledLayerNames = m_validationLayers.data();
        }

        auto extensions = getRequiredExtensions();
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

        VkResult result = vkCreateInstance(
                &createInfo,
                nullptr, // pointer to a custom allocator callback
                &m_instance
        );

        if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to create Vulkan instance");
        }

        // VK Extensions
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data());

        std::cout << "available extensions:" << std::endl;
        for (const auto& extension : availableExtensions) {
            std::cout << "\t" << extension.extensionName << std::endl;
        }
    }

    std::vector<const char*> getRequiredExtensions() {
        std::vector<const char*> extensions;

        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        for (uint32_t i = 0; i < glfwExtensionCount; i++) {
            extensions.push_back(glfwExtensions[i]);
        }

        if (enableValidationLayers) {
            extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
        }

        return extensions;
    }

    bool checkValidationLayerSupport() {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* layerName : m_validationLayers) {
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                return false;
            }
        }

        return true;
    }

    void setupDebugCallback() {
        if (!enableValidationLayers) {
            return;
        }

        // DEBUG
        VkDebugReportCallbackCreateInfoEXT createInfo = {
                .sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT,
                .flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT,
                .pfnCallback = debugCallback,
        };

        if (createDebugReportCallbackEXT(m_instance, &createInfo, nullptr, &m_callback) != VK_SUCCESS) {
            throw std::runtime_error("failed to set up debug callback!");
        }
    }

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugReportFlagsEXT flags,
            VkDebugReportObjectTypeEXT objType,
            uint64_t obj,
            size_t location,
            int32_t code,
            const char* layerPrefix,
            const char* msg,
            void* userData
    ) {
        std::cerr << "validation layer: " << msg << std::endl;

        return VK_FALSE;
    }
};

int main() {
    HelloTriangleApplication app;

    try {
        app.Run();
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
