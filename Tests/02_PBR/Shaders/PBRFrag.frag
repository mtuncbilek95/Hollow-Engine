#version 460

#include "BRDF.glsl"
#include "Lights.glsl"
#include "Common.glsl"

layout(location = 0) in vec3 fPosition;
layout(location = 1) in vec3 fWorldPosition;
layout(location = 2) in vec3 fNormal;
layout(location = 3) in vec2 fTexCoord;
layout(location = 4) in mat3 fTBN;

layout(location = 0) out vec4 outputColor;

layout(set = 1, binding = 0) uniform sampler2D albedoTex;
layout(set = 1, binding = 1) uniform sampler2D normalTex;
layout(set = 1, binding = 2) uniform sampler2D metallicTex;
layout(set = 1, binding = 3) uniform sampler2D ambientTex;
layout(set = 1, binding = 4) uniform sampler2D emissiveTex;
layout(set = 1, binding = 5) uniform samplerCube skyboxTex;

layout(set = 2, binding = 0) uniform UniformBufferObject
{
	vec3 camPos;
} ubo;

const vec3 lightPos = vec3(-100.0, 100.0, -100.0);
const vec3 lightColor = vec3(0.9, 1.0, 0.8);
const float lightPow = 15000.0;
const float EPSILON = 0.0001;

layout(push_constant) uniform PushForSkybox
{
	layout(offset = 128) mat4 view;
	mat4 proj;
} push;

vec3 GetSkyboxReflection(vec3 V, vec3 N, samplerCube skyboxTex)
{
	// Reflect the view vector
	vec3 R = reflect(mat3(push.view) * V, N);

	// Get the color from the skybox texture
	vec4 reflectionColor = texture(skyboxTex, R);

	return reflectionColor.rgb;
};

void main()
{
	vec4 albedoColor = texture(albedoTex, fTexCoord);
	vec3 normal = DecodeNormal(texture(normalTex, fTexCoord).xyz); // Decoded Normal Map
	float metallic = texture(metallicTex, fTexCoord).b;
	float roughness = texture(metallicTex, fTexCoord).g;
	float ambient = texture(ambientTex, fTexCoord).r;
	vec3 emissive = texture(emissiveTex, fTexCoord).rgb;

	vec3 N = normalize(fTBN * normal); // Normal vector 

	vec3 baseReflectivity = vec3(0.04); // Fresnel reflectance at normal incidence
	vec3 F0 = mix(baseReflectivity, albedoColor.rgb, metallic); // Mix F0 based on roughness value

	vec3 Lo = vec3(0.0); // Outgoing radiance

	vec3 V = normalize(ubo.camPos - fWorldPosition); // View vector
	vec3 L = normalize(lightPos - fWorldPosition); // Light vector
	vec3 H = normalize(V + L); // Halfway vector

	float Dist = length(lightPos - fWorldPosition); // Distance between light and fragment
	float Attenuation = 1.0 / (Dist * Dist); // Inverse square law
	vec3 Radiance = lightPow * Attenuation * lightColor; // Light radiance

	float NDF = DistributionGGX(N, H, roughness); // Normal Distribution Function
	float G = GeometrySmith(N, V, L, roughness); // Geometry Function
	vec3 F = FresnelSchlick(max(dot(H, V), 0.0), F0); // Fresnel

	vec3 refColor = GetSkyboxReflection(V, N, skyboxTex); // Skybox reflection color

	vec3 ks = F; // Specular reflection
	vec3 kd = (1.0 - ks) * (1.0 - metallic); // Diffuse reflection
	kd *= 1.0 - metallic; // Remove metallic from diffuse reflection

	vec3 numerator = NDF * G * F;
	float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + EPSILON;
	vec3 specular = numerator / denominator;

	float NdotL = max(dot(N, L), 0.0); // Dot product of normal and light vectors
	vec3 ambientColor = ambient * albedoColor.rgb; // Ambient term

	// Add diffuse and specular to outgoing radiance
	Lo += (kd * albedoColor.rgb / PI + specular) * Radiance * NdotL;

	vec3 reflectionContribution = refColor * ks;
	Lo += reflectionContribution * (1.0 - roughness);

	vec3 lastColor = Lo + ambientColor + emissive;

	AmbientLight ambLight;
	ambLight.Color = vec4(0.5, 0.5, 0.5, 1.0);
	ambLight.Intensity = 1.0;

	vec3 ambientLight = GetAmbientLight(ambLight).rgb;

	outputColor = vec4(lastColor * ambientLight, 1.0);
};


