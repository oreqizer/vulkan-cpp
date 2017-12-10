#include <vector>
#include <ios>
#include <fstream>

#include "pipeline.h"

namespace {
    std::vector<char> loadShader(const std::string& filename) {
        std::ifstream ifs(filename, std::ios::binary);
        std::vector<char> buffer{
                std::istreambuf_iterator<char>(ifs),
                std::istreambuf_iterator<char>()
        };

        return buffer;
    }

    VkShaderModule createShaderModule(VkDevice device, const std::vector<char>& code) {
        VkShaderModuleCreateInfo createInfo = {
                .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
                .codeSize = code.size(),
                .pCode = reinterpret_cast<const uint32_t*>(code.data()),
        };

        VkShaderModule shaderModule;
        if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
            throw std::runtime_error("failed to create shader module!");
        }

        return shaderModule;
    }
}

void pipeline::create(VkDevice device) {
    auto vertShaderCode = loadShader("../spv/vert.spv");
    auto fragShaderCode = loadShader("../spv/frag.spv");

    auto vertShader = createShaderModule(device, vertShaderCode);
    auto fragShader = createShaderModule(device, fragShaderCode);

    // .pSpecializationInfo (optional) allows specifying shader consts
    VkPipelineShaderStageCreateInfo vertShaderStageInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage = VK_SHADER_STAGE_VERTEX_BIT,
            .module = vertShader,
            .pName = "main",
    };

    VkPipelineShaderStageCreateInfo fragShaderStageInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
            .module = fragShader,
            .pName = "main",
    };

    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

    // ...fixed functions

    vkDestroyShaderModule(device, vertShader, nullptr);
    vkDestroyShaderModule(device, fragShader, nullptr);
}

void pipeline::destroy() {

}
