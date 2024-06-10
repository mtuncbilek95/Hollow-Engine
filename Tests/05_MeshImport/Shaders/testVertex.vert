#version 450
#extension GL_KHR_vulkan_glsl : enable

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;

layout(location = 0) out vec2 fragTexCoord;

layout(set = 0, binding = 0) uniform UniformBufferObject {
    mat4 model[49];
    mat4 view;
    mat4 proj;
} ubo;

void main()
{
	gl_Position = ubo.proj * ubo.view * ubo.model[gl_InstanceIndex] * vec4(inPosition.x, -inPosition.y, inPosition.z, 1.0);
    fragTexCoord = inTexCoord;
}