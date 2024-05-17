#version 450

layout(location = 0) in vec4 vertColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 fragColor;

layout(binding = 1) uniform sampler samplerTest;
layout(binding = 2) uniform texture2D textureTest;

void main()
{
    fragColor = texture(sampler2D(textureTest, samplerTest), fragTexCoord);
}