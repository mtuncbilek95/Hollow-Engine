#version 450
#extension GL_KHR_vulkan_glsl : enable

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec4 fragColor;
layout(location = 2) out vec2 fragTexCoord;

layout(set = 0, binding = 0) uniform UniformBufferObject {
    mat4 model[225];
    mat4 view;
    mat4 proj;
} mvpUbo;

void main()
{
	gl_Position = mvpUbo.proj * mvpUbo.view * mvpUbo.model[gl_InstanceIndex] * vec4(inPosition.x, -inPosition.y, inPosition.z, 1.0);
	fragColor = vec4(inColor, 1.0);
    fragTexCoord = inTexCoord;
}