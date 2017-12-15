#ifndef VULKAN_CPP_VERTEX_H
#define VULKAN_CPP_VERTEX_H

#include <vector>
#include <array>
#include <glm/glm.hpp>
#include <vulkan/vulkan.h>

struct Vertex {
    Vertex(glm::vec3 pos, glm::vec3 color);
    ~Vertex();

    const glm::vec3 pos;
    const glm::vec3 color;
};

namespace vertex {
    VkBuffer createBuffer(VkDevice device);
    void destroyBuffer(VkDevice device, VkBuffer buffer, VkDeviceMemory memory);
    VkDeviceMemory createMemory(VkPhysicalDevice physicalDevice, VkDevice device, VkBuffer buffer);
    VkVertexInputBindingDescription getBindingDescription();
    std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions();
}

#endif //VULKAN_CPP_VERTEX_H
