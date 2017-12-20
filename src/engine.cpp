#include "engine.h"

// TODO load from file
const std::vector<Vertex> vertices = {
        {{0.0f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}},
        {{0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}}
};

Engine::Engine() {
    m_window = initWindow();
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
    m_vertexBuffer = vertex::createBuffer(m_physicalDevice, m_device, vertices);
    m_commandBuffers = commands::createBuffers(
            m_device,
            m_swapchainExtent,
            m_renderPass,
            m_pipeline,
            m_framebuffers,
            m_commandPool,
            *m_vertexBuffer,
            vertices,
            m_framebuffers.size()
    );

    m_semaphoreImageAvailable = semaphore::create(m_device);
    m_semaphoreRenderFinished = semaphore::create(m_device);
}

Engine::~Engine() {
    semaphore::destroy(m_device, m_semaphoreRenderFinished);
    semaphore::destroy(m_device, m_semaphoreImageAvailable);
    swapchainCleanup();
    commands::destroyPool(m_device, m_commandPool);
    devices::destroyLogical(m_device);
    debug::destroyCallback(m_instance, m_callback);
    surface::destroy(m_instance, m_surface);
    instance::destroy(m_instance);
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Engine::Run() {
    while (!glfwWindowShouldClose(m_window)) {
        glfwPollEvents();
        auto result = frame::draw(
                m_device,
                m_graphicsQueue,
                m_presentQueue,
                m_swapchain,
                m_commandBuffers,
                m_semaphoreImageAvailable,
                m_semaphoreRenderFinished
        );

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
            swapchainRecreate();
        }
    }

    // frame::draw has async operations, this waits for them
    vkDeviceWaitIdle(m_device);
}

void Engine::onWindowResize(GLFWwindow *window, int width, int height) {
    if (width == 0 || height == 0) {
        return;
    }

    auto* app = reinterpret_cast<Engine*>(glfwGetWindowUserPointer(window));
    app->swapchainRecreate();
}

GLFWwindow* Engine::initWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // tells GLFW not to enable OpenGL context
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    auto window = glfwCreateWindow(
            WIDTH,
            HEIGHT,
            "Vulkan",
            nullptr, // monitor to open the window on
            nullptr  // something for OpenGL
    );

    glfwSetWindowUserPointer(window, this);
    glfwSetWindowSizeCallback(window, Engine::onWindowResize);

    return window;
}

void Engine::swapchainRecreate() {
    vkDeviceWaitIdle(m_device);

    swapchainCleanup();

    int width = 0;
    int height = 0;
    glfwGetWindowSize(m_window, &width, &height);
    auto swapchainData = swapchain::setup(
            m_surface,
            m_physicalDevice,
            m_device,
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
    );
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
    m_commandBuffers = commands::createBuffers(
            m_device,
            m_swapchainExtent,
            m_renderPass,
            m_pipeline,
            m_framebuffers,
            m_commandPool,
            *m_vertexBuffer,
            vertices,
            m_framebuffers.size()
    );
}

void Engine::swapchainCleanup() {
    commands::destroyBuffers(m_device, m_commandPool, m_commandBuffers); // reusing pool here
    framebuffers::destroy(m_device, m_framebuffers);
    pipeline::destroy(m_device, m_pipelineLayout, m_pipeline);
    pipeline::destroyRenderPass(m_device, m_renderPass);
    views::destroy(m_device, m_swapchainImageViews);
    swapchain::destroy(m_device, m_swapchain);
}