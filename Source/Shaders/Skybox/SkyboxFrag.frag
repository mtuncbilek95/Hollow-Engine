#version 460

layout(location = 0) in vec3 fTexCoord;

layout(set = 0, binding = 0) uniform samplerCube uSkybox;

layout(location = 0) out vec4 fragColor;

void main()
{
	fragColor = texture(uSkybox, fTexCoord);
};