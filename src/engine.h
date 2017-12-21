#ifndef VULKAN_CPP_ENGINE_H
#define VULKAN_CPP_ENGINE_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <map>
#include <set>

#include "buffer.h"
#include "debug.h"
#include "instance.h"
#include "surface.h"
#include "device.h"
#include "swapchain.h"
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

    Device* device_;
    Swapchain* swapchain_;

    VkRenderPass m_renderPass;
    VkPipelineLayout m_pipelineLayout;
    VkPipeline m_pipeline;

    std::vector<VkFramebuffer> m_framebuffers;
    VkCommandPool m_commandPool;
    Buffer* m_vertexBuffer; // TODO make this by reference
    std::vector<VkCommandBuffer> m_commandBuffers;

    VkSemaphore m_semaphoreImageAvailable;
    VkSemaphore m_semaphoreRenderFinished;

    static void onWindowResize(GLFWwindow* window, int width, int height);

    GLFWwindow* initWindow();
    void swapchainRecreate();
    void swapchainCleanup();
};

#endif //VULKAN_CPP_ENGINE_H
