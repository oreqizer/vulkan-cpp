#include <stdexcept>

#include "surface.h"

VkSurfaceKHR surface::create(VkInstance instance, GLFWwindow* window) {
    VkSurfaceKHR surface;

    if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }

    return surface;
}

void surface::destroy(VkInstance instance, VkSurfaceKHR surface) {
    vkDestroySurfaceKHR(instance, surface, nullptr);
}
