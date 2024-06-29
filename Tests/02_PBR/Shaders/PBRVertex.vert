#version 460
#extension GL_KHR_vulkan_glsl : enable

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec3 vTangent;
layout(location = 3) in vec3 vBitangent;
layout(location = 4) in vec4 vColor;
layout(location = 5) in vec2 vTexCoord;

layout(location = 0) out vec3 fPosition;
layout(location = 1) out vec3 fNormal;
layout(location = 2) out vec3 fTangent;
layout(location = 3) out vec3 fBitangent;
layout(location = 4) out vec2 fTexCoord;

layout(std140, set = 0, binding = 0) uniform ObjectModel {
    mat4 model;
} ubo;

layout(std140, push_constant) uniform CameraMatrix
{
	mat4 view;
	mat4 proj;
} camConst;

void main()
{
	fPosition = vec3(ubo.model * vec4(vPosition, 1.0));
	fNormal = vec3(ubo.model * vec4(vNormal, 1.0));
	fTangent = vec3(ubo.model * vec4(vTangent, 1.0));
	fBitangent = vec3(ubo.model * vec4(vBitangent, 1.0));
	fTexCoord = vTexCoord;
	gl_Position = camConst.proj * camConst.view * ubo.model * vec4(vPosition, 1.0);
}