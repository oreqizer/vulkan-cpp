#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include <iostream>
#include <stdexcept>
#include <functional>

class HelloTriangleApplication {
public:
    void Run() {
        initVulkan();
        mainLoop();
        cleanup();
    }

private:
    void initVulkan() {

    }

    void mainLoop() {

    }

    void cleanup() {

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
