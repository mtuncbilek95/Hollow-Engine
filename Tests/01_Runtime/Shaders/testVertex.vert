#version 450
#extension GL_KHR_vulkan_glsl : enable
#extension GL_GOOGLE_include_directive : enable

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec2 fragTexCoord;

layout(std140, set = 0, binding = 0) uniform UniformBufferObject {
    mat4 model[1];
} ubo;

layout(std140, push_constant) uniform Constants
{
	mat4 view;
	mat4 proj;
} constants;

void main()
{
	gl_Position = constants.proj * constants.view * ubo.model[gl_InstanceIndex] * vec4(inPosition.x, -inPosition.y, inPosition.z, 1.0);
	fragColor = vec4(inColor, 1.0);
    fragTexCoord = inTexCoord;
}