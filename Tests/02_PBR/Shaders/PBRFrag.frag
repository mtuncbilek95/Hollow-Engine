#version 460

#include "BRDF.glsl"
#include "Common.glsl"

layout(location = 0) in vec3 fPosition;
layout(location = 1) in vec3 fNormal;
layout(location = 2) in vec3 fTangent;
layout(location = 3) in vec3 fBitangent;
layout(location = 4) in vec2 fTexCoord;

layout(location = 0) out vec4 outputColor;

layout(set = 1, binding = 0) uniform sampler2D albedoTex;
layout(set = 1, binding = 1) uniform sampler2D normalTex;
layout(set = 1, binding = 2) uniform sampler2D metallicTex;
layout(set = 1, binding = 3) uniform sampler2D ambientTex;
layout(set = 1, binding = 4) uniform sampler2D emissiveTex;

void main()
{
	vec4 albedoColor = texture(albedoTex, fTexCoord);
	vec3 normal = texture(normalTex, fTexCoord).xyz;
	float metallic = texture(metallicTex, fTexCoord).r;
	float ambient = texture(ambientTex, fTexCoord).r;
	vec3 emissive = texture(emissiveTex, fTexCoord).rgb;

	outputColor = albedoColor + vec4(emissive, 1.0);
};


