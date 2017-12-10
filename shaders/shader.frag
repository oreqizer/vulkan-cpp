#version 450
#extension GL_ARB_separate_shader_objects : enable // required for Vulkan

layout(location = 0) out vec4 outColor; // output variable
layout(location = 0) in vec3 fragColor; // input variable

void main() {
    outColor = vec4(fragColor, 1.0); // orange
}
