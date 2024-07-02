#version 460
#extension GL_KHR_vulkan_glsl : enable

layout(location = 0) in vec3 vPosition;
layout(location = 0) out vec3 fTexCoord;

layout(std140, push_constant) uniform CameraMatrix
{
	mat4 view;
	mat4 proj;
} camConst;

void main()
{
	gl_Position = camConst.proj * camConst.view * vec4(vPosition, 1.0);
	fTexCoord = vPosition;
}