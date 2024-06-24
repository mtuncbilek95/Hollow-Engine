#version 460
#extension GL_KHR_vulkan_glsl : enable
#extension GL_GOOGLE_include_directive : enable

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inTangent;
layout(location = 3) in vec3 inBitangent;
layout(location = 4) in vec4 inColor;
layout(location = 5) in vec2 inTexCoord;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec3 fragNormal;
layout(location = 3) out vec3 fragPosition;

layout(std140, set = 0, binding = 0) uniform UniformBufferObject {
    mat4 model;
} ubo;

layout(std140, push_constant) uniform Constants
{
	mat4 view;
	mat4 proj;
} constants;

void main()
{
	gl_Position = constants.proj * constants.view * ubo.model * vec4(inPosition.x, inPosition.y, inPosition.z, 1.0);
	fragColor = inColor;
    fragTexCoord = inTexCoord;
	fragNormal = vec3(ubo.model * vec4(inNormal, 1.0));
	fragPosition = vec3(ubo.model * vec4(inPosition, 1.0));
}