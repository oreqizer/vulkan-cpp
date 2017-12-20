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
    struct Data {
        VkBuffer buffer;
        VkDeviceMemory memory;
    };

    Data createBuffer(VkPhysicalDevice physicalDevice, VkDevice device, std::vector<Vertex> vertices);
    void destroyBuffer(VkDevice device, VkDeviceMemory memory, VkBuffer buffer);
    VkVertexInputBindingDescription getBindingDescription();
    std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions();
}

#endif //VULKAN_CPP_VERTEX_H
