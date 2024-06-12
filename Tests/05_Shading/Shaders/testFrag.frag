#version 450

layout(location = 0) in vec2 fragTexCoord;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec3 fragPosition;

layout(location = 0) out vec4 fragColor;

layout(binding = 1) uniform sampler samplerTest;
layout(binding = 2) uniform texture2D textureTest;
layout(binding = 3) uniform texture2D normalMap;

layout(binding = 4) uniform AmbientLight
{
	vec3 color;
	float ambientIntensity;
} uboAmb;

layout(binding = 5) uniform DiffuseLight
{
	vec3 position;
	vec3 color;
    float intensity;
} uboDif;

vec3 decodeNormal(vec3 n)
{
    return normalize(n * 2.0 - 1.0);
}

void main() {
    // Sample the texture color
    vec4 texColor = texture(sampler2D(textureTest, samplerTest), fragTexCoord);

    fragColor = texColor;
}