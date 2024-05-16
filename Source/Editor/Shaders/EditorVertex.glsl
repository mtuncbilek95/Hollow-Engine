#version 450
#extension GL_KHR_vulkan_glsl : enable

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec2 inTexCoord;
layout(location = 2) in vec4 inColor;

layout(location = 0) out vec2 outTexCoord;
layout(location = 1) out vec4 outColor;

layout(set = 0, binding = 0) uniform ImguiUbo {
    mat4 model;
    mat4 view;
    mat4 proj;
} ImguiUbo;

void main()
{
	gl_Position = ImguiUbo.proj * ImguiUbo.view * ImguiUbo.model * vec4(inPosition, 0.0, 1.0);
	outColor = inColor;
}