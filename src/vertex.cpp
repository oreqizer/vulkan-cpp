#include "vertex.h"

Vertex::Vertex(glm::vec3 pos, glm::vec3 color): pos(pos), color(color) {
    // ctor
}

Vertex::~Vertex() =default;

VkBuffer vertex::create(VkDevice device) {
    // TODO get rid of this, make it a param
    const std::vector<Vertex> vertices = {
            {{0.0f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}}
    };

    VkBufferCreateInfo bufferInfo = {
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .size = sizeof(vertices[0]) * vertices.size(),
            .usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    };

    VkBuffer buffer;
    if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create vertex buffer!");
    }

    return buffer;
}

void vertex::destroy(VkDevice device, VkBuffer buffer) {
    vkDestroyBuffer(device, buffer, nullptr);
}

VkVertexInputBindingDescription vertex::getBindingDescription() {
    VkVertexInputBindingDescription description = {
            .binding = 0, // index in the array of bindings
            .stride = sizeof(Vertex), // number fo bytes from one entry to the next
            .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
    };

    return description;
}

std::array<VkVertexInputAttributeDescription, 2> vertex::getAttributeDescriptions() {
    std::array<VkVertexInputAttributeDescription, 2> descriptions = {
            VkVertexInputAttributeDescription{
                    // from which binding the per-vertex data comes
                    .binding = 0,
                    // location directive of the input in the vertex shader
                    .location = 0,
                    .format = VK_FORMAT_R32G32B32_SFLOAT,
                    // // the number of bytes since the start of the per-vertex data to read from
                    .offset = static_cast<uint32_t>(offsetof(Vertex, pos)),
            },
            VkVertexInputAttributeDescription{
                    .binding = 0,
                    .location = 1,
                    .format = VK_FORMAT_R32G32B32_SFLOAT,
                    .offset = static_cast<uint32_t>(offsetof(Vertex, color)),
            }
    };

    return descriptions;
}