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

VkRenderPass pipeline::createRenderPass(VkDevice device, VkFormat format) {
    VkAttachmentDescription colorAttachment = {
            .format = format,
            .samples = VK_SAMPLE_COUNT_1_BIT,
            .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
            .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
    };

    // index used in `layout(location = 0) out vec4 outColor`
    // in the fragment shader
    VkAttachmentReference colorAttachmentRef = {
            .attachment = 0,
            .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    };

    VkSubpassDescription subpass = {
            .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
            .colorAttachmentCount = 1,
            .pColorAttachments = &colorAttachmentRef,
    };

    VkRenderPassCreateInfo renderPassInfo = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
            .attachmentCount = 1,
            .pAttachments = &colorAttachment,
            .subpassCount = 1,
            .pSubpasses = &subpass,
    };

    VkRenderPass renderPass;
    if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass!");
    }

    return renderPass;
}

void pipeline::destroyRenderPass(VkDevice device, VkRenderPass renderPass) {
    vkDestroyRenderPass(device, renderPass, nullptr);
}

pipeline::Data pipeline::create(VkDevice device, VkExtent2D extent, VkRenderPass renderPass) {
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

    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
            .vertexBindingDescriptionCount = 0,
            .pVertexBindingDescriptions = nullptr, // optional
            .vertexAttributeDescriptionCount = 0,
            .pVertexAttributeDescriptions = nullptr, // optional
    };

    VkPipelineInputAssemblyStateCreateInfo assemblyInputInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
            .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
            .primitiveRestartEnable = VK_FALSE,
    };

    VkViewport viewport = {
            .x = 0.0f,
            .y = 0.0f,
            .width = (float) extent.width,
            .height = (float) extent.height,
            .minDepth = 0.0f,
            .maxDepth = 1.0f,
    };

    VkRect2D scissor = {
            .offset = {0, 0},
            .extent = extent,
    };

    VkPipelineViewportStateCreateInfo viewportInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
            .viewportCount = 1,
            .pViewports = &viewport,
            .scissorCount = 1,
            .pScissors = &scissor,
    };

    VkPipelineRasterizationStateCreateInfo rasterizerInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
            .depthClampEnable = VK_FALSE,
            .rasterizerDiscardEnable = VK_FALSE,
            .polygonMode = VK_POLYGON_MODE_FILL,
            .lineWidth = 1.0f,
            .cullMode = VK_CULL_MODE_BACK_BIT,
            .frontFace = VK_FRONT_FACE_CLOCKWISE,
            .depthBiasEnable = VK_FALSE,
            .depthBiasConstantFactor = 0.0f, // optional
            .depthBiasClamp = 0.0f, // optional
            .depthBiasSlopeFactor = 0.0f, // optional
    };

    VkPipelineMultisampleStateCreateInfo multisamplingInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
            .sampleShadingEnable = VK_FALSE,
            .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
            .minSampleShading = 1.0f, // optional
            .pSampleMask = nullptr, // optional
            .alphaToCoverageEnable = VK_FALSE, // optional
            .alphaToOneEnable = VK_FALSE, // optional
    };

    VkPipelineColorBlendAttachmentState colorBlendAttachment = {
            .colorWriteMask =
                VK_COLOR_COMPONENT_R_BIT |
                VK_COLOR_COMPONENT_G_BIT |
                VK_COLOR_COMPONENT_B_BIT |
                VK_COLOR_COMPONENT_A_BIT,
            .blendEnable = VK_FALSE,
            .srcColorBlendFactor = VK_BLEND_FACTOR_ONE, // optional
            .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO, // optional
            .colorBlendOp = VK_BLEND_OP_ADD, // optional
            .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE, // optional
            .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO, // optional
            .alphaBlendOp = VK_BLEND_OP_ADD, // optional
            // pseudocode of that the following setting does:
            // finalColor.rgb = newAlpha * newColor + (1 - newAlpha) * oldColor;
            // finalColor.a = newAlpha.a;
            // .blendEnable = VK_TRUE,
            // .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
            // .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
            // .colorBlendOp = VK_BLEND_OP_ADD,
            // .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
            // .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
            // .alphaBlendOp = VK_BLEND_OP_ADD,
    };

    VkPipelineColorBlendStateCreateInfo colorBlendInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
            .logicOpEnable = VK_FALSE,
            .logicOp = VK_LOGIC_OP_COPY, // optional
            .attachmentCount = 1,
            .pAttachments = &colorBlendAttachment,
            .blendConstants[0] = 0.0f, // optional
            .blendConstants[1] = 0.0f, // optional
            .blendConstants[2] = 0.0f, // optional
            .blendConstants[3] = 0.0f, // optional
    };

    // example of specifying dynamically adjustable values:
    //
    // VkDynamicState dynamicStates[] = {
    //         VK_DYNAMIC_STATE_VIEWPORT,
    //         VK_DYNAMIC_STATE_LINE_WIDTH
    // };

    // VkPipelineDynamicStateCreateInfo dynamicState = {};
    // dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    // dynamicState.dynamicStateCount = 2;
    // dynamicState.pDynamicStates = dynamicStates;

    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .setLayoutCount = 0, // optional
            .pSetLayouts = nullptr, // optional
            .pushConstantRangeCount = 0, // optional
            .pPushConstantRanges = 0, // optional
    };

    pipeline::Data data = {};
    if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &data.layout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }

    // cleanup shader modules
    vkDestroyShaderModule(device, vertShader, nullptr);
    vkDestroyShaderModule(device, fragShader, nullptr);

    VkGraphicsPipelineCreateInfo pipelineInfo = {
            .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
            .stageCount = 2,
            .pStages = shaderStages,
            .pVertexInputState = &vertexInputInfo,
            .pInputAssemblyState = &assemblyInputInfo,
            .pViewportState = &viewportInfo,
            .pRasterizationState = &rasterizerInfo,
            .pMultisampleState = &multisamplingInfo,
            .pDepthStencilState = nullptr, // optional
            .pColorBlendState = &colorBlendInfo,
            .pDynamicState = nullptr, // optional
            .layout = data.layout,
            .renderPass = renderPass,
            .subpass = 0,
            .basePipelineHandle = VK_NULL_HANDLE, // optional
            .basePipelineIndex = -1, // optional
    };

    auto state = vkCreateGraphicsPipelines(
            device,
            VK_NULL_HANDLE, // optional cache object
            1,              // number of creation infos
            &pipelineInfo,
            nullptr,
            &data.instance
    );

    if (state != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics pipeline!");
    }

    return data;
}

void pipeline::destroy(VkDevice device, VkPipelineLayout layout, VkPipeline pipeline) {
    vkDestroyPipelineLayout(device, layout, nullptr);
    vkDestroyPipeline(device, pipeline, nullptr);
}
