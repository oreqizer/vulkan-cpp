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

    GLFWwindow* m_window;
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
