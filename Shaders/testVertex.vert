#version 450

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec4 fragColor;

layout(binding = 0) uniform UniformBufferObject
{
	mat4 model;
	mat4 view;
	mat4 proj;

} ubo;

void main()
{
	gl_Position = vec4(inPosition.x, -inPosition.y, 0.0, 1.0);
	fragColor = vec4(inColor, 1.0);
}