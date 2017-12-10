#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <map>
#include <set>

#include "src/debug.h"
#include "src/instance.h"
#include "src/surface.h"
#include "src/devices.h"
#include "src/swapchain.h"
#include "src/views.h"
#include "src/pipeline.h"
#include "src/framebuffers.h"
#include "src/commands.h"

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
    VkDebugReportCallbackEXT m_callback;
    VkSurfaceKHR m_surface;
    VkPhysicalDevice m_physicalDevice;

    VkDevice m_device;
    VkQueue m_graphicsQueue;
    VkQueue m_presentQueue;

    VkSwapchainKHR m_swapChain;
    std::vector<VkImage> m_swapChainImages;
    VkFormat m_swapChainImageFormat;
    VkExtent2D m_swapChainExtent;

    std::vector<VkImageView> m_swapChainImageViews;

    VkRenderPass m_renderPass;
    VkPipelineLayout m_pipelineLayout;
    VkPipeline m_pipeline;

    std::vector<VkFramebuffer> m_framebuffers;
    VkCommandPool m_commandPool;
    std::vector<VkCommandBuffer> m_commandBuffers;

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
        m_instance = instance::create();
        m_callback = debug::setupCallback(m_instance);
        m_surface = surface::create(m_instance, m_window);
        m_physicalDevice = devices::pickPhysical(m_instance, m_surface);

        auto deviceData = devices::createLogical(m_surface, m_physicalDevice);
        m_device = deviceData.device;
        m_graphicsQueue = deviceData.graphicsQueue;
        m_presentQueue = deviceData.presentQueue;

        auto swapchainData = swapchain::setup(m_surface, m_physicalDevice, m_device, WIDTH, HEIGHT);
        m_swapChain = swapchainData.instance;
        m_swapChainImages = swapchainData.images;
        m_swapChainImageFormat = swapchainData.format;
        m_swapChainExtent = swapchainData.extent;

        m_swapChainImageViews = views::create(m_device, m_swapChainImages, m_swapChainImageFormat);
        m_renderPass = pipeline::createRenderPass(m_device, m_swapChainImageFormat);

        auto pipelineData = pipeline::create(m_device, m_swapChainExtent, m_renderPass);
        m_pipelineLayout = pipelineData.layout;
        m_pipeline = pipelineData.instance;

        m_framebuffers = framebuffers::create(m_device, m_swapChainExtent, m_swapChainImageViews, m_renderPass);
        m_commandPool = commands::createPool(m_surface, m_physicalDevice, m_device);
        m_commandBuffers = commands::createBuffers(m_device);
    }

    void mainLoop() {
        while (!glfwWindowShouldClose(m_window)) {
            glfwPollEvents();
        }
    }

    void cleanup() {
        commands::destroyBuffers(m_device, m_commandBuffers);
        commands::destroyPool(m_device, m_commandPool);
        framebuffers::destroy(m_device, m_framebuffers);
        pipeline::destroy(m_device, m_pipelineLayout, m_pipeline);
        pipeline::destroyRenderPass(m_device, m_renderPass);
        views::destroy(m_device, m_swapChainImageViews);
        swapchain::destroy(m_device, m_swapChain);
        devices::destroyLogical(m_device);
        debug::destroyCallback(m_instance, m_callback);
        surface::destroy(m_instance, m_surface);
        instance::destroy(m_instance);
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
