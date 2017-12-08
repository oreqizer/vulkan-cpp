#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include <iostream>
#include <stdexcept>
#include <functional>

const int WIDTH = 800;
const int HEIGHT = 600;

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
    }

    void createInstance() {
        VkApplicationInfo appInfo = {
                sType: VK_STRUCTURE_TYPE_APPLICATION_INFO,
                pApplicationName: "Hello Triangle",
                applicationVersion: VK_MAKE_VERSION(1, 0, 0),
                pEngineName: "No Engine",
                engineVersion: VK_MAKE_VERSION(1, 0, 0),
                apiVersion: VK_API_VERSION_1_0,
        };

        uint32_t glfwExtensionCount = 0;
        const char ** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        VkInstanceCreateInfo createInfo = {
                sType: VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
                pApplicationInfo: &appInfo,
                enabledExtensionCount: glfwExtensionCount,
                ppEnabledExtensionNames: glfwExtensions,
                enabledLayerCount: 0,
        };

        VkResult result = vkCreateInstance(
                &createInfo,
                nullptr, // pointer to a custom allocator callback
                &m_instance
        );

        if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to create Vulkan instance");
        }
    }

    void mainLoop() {
        while (!glfwWindowShouldClose(m_window)) {
            glfwPollEvents();
        }
    }

    void cleanup() {
        glfwDestroyWindow(m_window);
        glfwTerminate();
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
