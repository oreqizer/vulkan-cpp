#ifndef VULKAN_CPP_VERTEX_H
#define VULKAN_CPP_VERTEX_H

#include <vector>
#include <array>
#include <glm/glm.hpp>
#include <vulkan/vulkan.h>

struct Vertex {
    Vertex(glm::vec3 po3, glm::vec3 color);
    ~Vertex();

    const glm::vec3 pos;
    const glm::vec3 color;
};

namespace vertex {
    VkBuffer create(VkDevice device);
    void destroy(VkDevice device, VkBuffer buffer);

    VkVertexInputBindingDescription getBindingDescription();
    std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions();
}

#endif //VULKAN_CPP_VERTEX_H
