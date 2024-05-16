#version 450
#extension GL_KHR_vulkan_glsl : enable

layout(location = 0) in vec2 inTexCoord;
layout(location = 1) in vec4 inColor;

layout(location = 0) out vec4 fragColor;

layout(set = 0, binding = 1) uniform sampler samplerTest;
layout(set = 1, binding = 0) uniform texture2D textureTest;

void main()
{
    fragColor = inColor * texture(sampler2D(textureTest, samplerTest), fragTexCoord);
}