#ifndef VULKAN_CPP_SURFACE_H
#define VULKAN_CPP_SURFACE_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace surface {
    VkSurfaceKHR create(VkInstance instance, GLFWwindow* window);
    void destroy(VkInstance instance, VkSurfaceKHR surface);
}

#endif //VULKAN_CPP_SURFACE_H
