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
#include "src/semaphore.h"
#include "src/frame.h"

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

    VkSwapchainKHR m_swapchain;
    std::vector<VkImage> m_swapchainImages;
    VkFormat m_swapchainImageFormat;
    VkExtent2D m_swapchainExtent;

    std::vector<VkImageView> m_swapchainImageViews;

    VkRenderPass m_renderPass;
    VkPipelineLayout m_pipelineLayout;
    VkPipeline m_pipeline;

    std::vector<VkFramebuffer> m_framebuffers;
    VkCommandPool m_commandPool;
    std::vector<VkCommandBuffer> m_commandBuffers;

    VkSemaphore m_semaphoreImageAvailable;
    VkSemaphore m_semaphoreRenderFinished;

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
        m_swapchain = swapchainData.instance;
        m_swapchainImages = swapchainData.images;
        m_swapchainImageFormat = swapchainData.format;
        m_swapchainExtent = swapchainData.extent;

        m_swapchainImageViews = views::create(m_device, m_swapchainImages, m_swapchainImageFormat);
        m_renderPass = pipeline::createRenderPass(m_device, m_swapchainImageFormat);

        auto pipelineData = pipeline::create(m_device, m_swapchainExtent, m_renderPass);
        m_pipelineLayout = pipelineData.layout;
        m_pipeline = pipelineData.instance;

        m_framebuffers = framebuffers::create(m_device, m_swapchainExtent, m_swapchainImageViews, m_renderPass);
        m_commandPool = commands::createPool(m_surface, m_physicalDevice, m_device);
        m_commandBuffers = commands::createBuffers(
                m_device,
                m_swapchainExtent,
                m_renderPass,
                m_pipeline,
                m_framebuffers,
                m_commandPool,
                m_framebuffers.size()
        );

        m_semaphoreImageAvailable = semaphore::create(m_device);
        m_semaphoreRenderFinished = semaphore::create(m_device);
    }

    void mainLoop() {
        while (!glfwWindowShouldClose(m_window)) {
            glfwPollEvents();
            frame::draw(
                    m_device,
                    m_graphicsQueue,
                    m_presentQueue,
                    m_swapchain,
                    m_commandBuffers,
                    m_semaphoreImageAvailable,
                    m_semaphoreRenderFinished
            );
        }

        // frame::draw has async operations, this waits for them
        vkDeviceWaitIdle(m_device);
    }

    void cleanup() {
        semaphore::destroy(m_device, m_semaphoreRenderFinished);
        semaphore::destroy(m_device, m_semaphoreImageAvailable);
        commands::destroyPool(m_device, m_commandPool);
        framebuffers::destroy(m_device, m_framebuffers);
        pipeline::destroy(m_device, m_pipelineLayout, m_pipeline);
        pipeline::destroyRenderPass(m_device, m_renderPass);
        views::destroy(m_device, m_swapchainImageViews);
        swapchain::destroy(m_device, m_swapchain);
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
