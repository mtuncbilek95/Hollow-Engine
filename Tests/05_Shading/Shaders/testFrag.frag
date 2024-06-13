#version 450

layout(location = 0) in vec2 fragTexCoord;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec3 fragPosition;

layout(location = 0) out vec4 fragColor;

layout(binding = 1) uniform sampler samplerTest;
layout(binding = 2) uniform texture2D textureTest;
layout(binding = 3) uniform texture2D normalMap;

layout(binding = 4) uniform DiffuseLight
{
	vec3 color;
	float ambientIntensity;
} uboAmb;

layout(binding = 5) uniform AmbientLight
{
	vec4 position;
	vec4 color;
} uboDif;

vec3 decodeNormal(vec3 n)
{
    return normalize(n * 2.0 - 1.0);
}

void main() {
    // Sample the texture color
    vec4 texColor = texture(sampler2D(textureTest, samplerTest), fragTexCoord);
	vec3 normalColor = decodeNormal(texture(sampler2D(normalMap, samplerTest), fragTexCoord).xyz);

	vec3 ambientLight = uboAmb.color * uboAmb.ambientIntensity;

	vec3 norm = normalize(fragNormal);
	vec3 lightDir = normalize(uboDif.position.xyz - fragPosition);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * uboDif.color.rgb;

	vec3 result = (ambientLight + diffuse) * texColor.rgb;

    fragColor = vec4(result, 1.0);
}