#version 460

#include "Lights/Lights.glsl"
#include "Common.glsl"

layout(location = 0) in vec4 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragNormal;
layout(location = 3) in vec3 fragPosition;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 1) uniform sampler samplerTest;
layout(set = 0, binding = 2) uniform texture2D textureTest;
layout(set = 0, binding = 3) uniform texture2D normalMap;

layout(set = 1, binding = 0) readonly buffer AmbientLights
{
	AmbientLight ambient;
	DiffuseLight diffuse;
} uboLights;

void main()
{
	vec4 texColor = texture(sampler2D(textureTest, samplerTest), fragTexCoord);
	float texAlpha = texColor.a;

	vec3 normColor = decodeNormal(texture(sampler2D(normalMap, samplerTest), fragTexCoord).xyz);
	vec3 ambientColor = GetAmbientLight(uboLights.ambient).rgb;

	vec3 diffuse = GetDiffuseLight(uboLights.diffuse, fragNormal, fragPosition);
	
	vec4 finalColor = vec4(texColor.rgb * (ambientColor + diffuse), texAlpha);

	if(finalColor.a < 0.1)
        discard;

    outColor = finalColor;
}