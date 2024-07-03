#version 460

#include "BRDF.glsl"
#include "Lights.glsl"
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

layout(set = 2, binding = 0) uniform UniformBufferObject
{
	vec3 camPos;
} ubo;

layout(set = 3, binding = 0) uniform samplerCube irradianceMap;

const vec3 lightPos = vec3(3.0, 3.0, -5.0);
const vec3 lightColor = vec3(0.8, 0.8, 1.0);
const float lightPow = 100.0;
const float EPSILON = 0.001;

void main()
{
	vec4 albedoColor = texture(albedoTex, fTexCoord);
	vec3 normal = DecodeNormal(texture(normalTex, fTexCoord).xyz); // Decoded Normal Map
	float metallic = texture(metallicTex, fTexCoord).b;
	float roughness = texture(metallicTex, fTexCoord).g;
	float ambient = texture(ambientTex, fTexCoord).r;
	vec3 emissive = texture(emissiveTex, fTexCoord).rgb;

	mat3 TBN = mat3(fTangent, fBitangent, fNormal); // Tangent space to world space matrix
	vec3 N = normalize(TBN * normal); // Normal vector 

	vec3 F0 = vec3(0.04); // Fresnel reflectance at normal incidence
	F0 = mix(F0, albedoColor.rgb, metallic); // Mix F0 based on metallic value

	vec3 Lo = vec3(0.0); // Outgoing radiance

	vec3 V = normalize(ubo.camPos - fPosition); // View vector
	vec3 L = normalize(lightPos - fPosition); // Light vector
	vec3 H = normalize(V + L); // Halfway vector

	float Dist = length(lightPos - fPosition); // Distance between light and fragment
	float Attenuation = 1.0 / (Dist * Dist); // Inverse square law
	vec3 Radiance = lightPow * Attenuation * lightColor; // Light radiance

	float NDF = DistributionGGX(N, H, roughness); // Normal Distribution Function
	float G = GeometrySmith(N, V, L, roughness); // Geometry Function
	vec3 F = FresnelSchlick(max(dot(H, V), 0.0), F0); // Fresnel

	vec3 ks = F; // Specular reflection
	vec3 kd = (1.0 - ks) * (1.0 - metallic); // Diffuse reflection
	kd *= 1.0 - metallic; // Remove metallic from diffuse reflection

	vec3 numerator = NDF * G * F;
	float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + EPSILON;
	vec3 specular = numerator / denominator;

	float NdotL = max(dot(N,L),0.0); // Dot product of normal and light vectors
	Lo +=(kd * albedoColor.rgb/PI + specular) * Radiance * NdotL; // Add diffuse and specular to outgoing radiance
	
	AmbientLight ambLight;
	ambLight.Color = vec4(1.0, 1.0, 1.0, 1.0);
	ambLight.Intensity = 0.1;

	vec3 ambientColor = (albedoColor.rgb) * ambient * GetAmbientLight(ambLight).rgb; // Ambient color
	vec3 color = ambientColor + Lo; // Final color

	outputColor = vec4(color + emissive, 1.0);
};


