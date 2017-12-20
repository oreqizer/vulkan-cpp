#include "vertex.h"

Vertex::Vertex(glm::vec3 pos, glm::vec3 color): pos(pos), color(color) {
    // ctor
}

Vertex::~Vertex() =default;

Buffer* vertex::createBuffer(Device& device, std::vector<Vertex> vertices) {
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
    auto buffer = new Buffer(
            device,
            bufferSize,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    );

    void* data;
    vkMapMemory(device.getLogical(), buffer->getMemory(), 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), (size_t) bufferSize);
    vkUnmapMemory(device.getLogical(), buffer->getMemory());

    return buffer;
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
