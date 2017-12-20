#include "vertex.h"
#include "devices.h"

Vertex::Vertex(glm::vec3 pos, glm::vec3 color): pos(pos), color(color) {
    // ctor
}

Vertex::~Vertex() =default;

vertex::Data vertex::createBuffer(VkPhysicalDevice physicalDevice, VkDevice device, std::vector<Vertex> vertices) {
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
    VkMemoryRequirements requirements = {};
    vkGetBufferMemoryRequirements(device, buffer, &requirements);

    VkMemoryAllocateInfo allocInfo = {
            .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .allocationSize = requirements.size,
            .memoryTypeIndex = devices::findMemoryType(
                    physicalDevice,
                    requirements.memoryTypeBits,
                    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
            ),
    };

    VkDeviceMemory memory;
    if (vkAllocateMemory(device, &allocInfo, nullptr, &memory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate vertex buffer memory!");
    }

    vkBindBufferMemory(device, buffer, memory, 0);

    void* data;
    vkMapMemory(device, memory, 0, bufferInfo.size, 0, &data);
    memcpy(data, vertices.data(), (size_t) bufferInfo.size);
    vkUnmapMemory(device, memory);

    return {
            .buffer = buffer,
            .memory = memory,
    };
}

void vertex::destroyBuffer(VkDevice device, VkDeviceMemory memory, VkBuffer buffer) {
    vkDestroyBuffer(device, buffer, nullptr);
    vkFreeMemory(device, memory, nullptr);
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
