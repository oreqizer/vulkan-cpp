#ifndef VULKAN_CPP_ENGINE_H
#define VULKAN_CPP_ENGINE_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <map>
#include <set>

#include "debug.h"
#include "instance.h"
#include "surface.h"
#include "devices.h"
#include "swapchain.h"
#include "views.h"
#include "pipeline.h"
#include "framebuffers.h"
#include "commands.h"
#include "semaphore.h"
#include "frame.h"
#include "vertex.h"

class Engine {
public:
    Engine();
    ~Engine();

    void Run();

private:
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;

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
    VkBuffer m_vertexBuffer;
    std::vector<VkCommandBuffer> m_commandBuffers;

    VkSemaphore m_semaphoreImageAvailable;
    VkSemaphore m_semaphoreRenderFinished;

    static void onWindowResize(GLFWwindow* window, int width, int height);

    GLFWwindow* initWindow();
    void swapchainRecreate();
    void swapchainCleanup();
};

#endif //VULKAN_CPP_ENGINE_H
