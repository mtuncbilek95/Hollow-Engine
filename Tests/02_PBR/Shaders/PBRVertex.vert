#version 460
#extension GL_KHR_vulkan_glsl : enable

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec3 vTangent;
layout(location = 3) in vec3 vBitangent;
layout(location = 4) in vec4 vColor;
layout(location = 5) in vec2 vTexCoord;

layout(location = 0) out vec3 fPosition;
layout(location = 1) out vec3 fWorldPosition;
layout(location = 2) out vec3 fNormal;
layout(location = 3) out vec2 fTexCoord;
layout(location = 4) out mat3 fTBN;

layout(std140, set = 0, binding = 0) uniform ObjectModel {
    mat4 model;
} ubo;

layout(std140, push_constant) uniform CameraMatrix
{
	mat4 view;
	mat4 proj;
} camConst;

void main()
{
	// fragment Position
	fPosition = vPosition;
	// fragment World Position
	fWorldPosition = vec3(ubo.model * vec4(vPosition, 1.0));
	// fragment Normal
	fNormal = mat3(transpose(inverse(ubo.model))) * vNormal; // regular normal matrix
	// fragment TexCoord
	fTexCoord = vTexCoord;

	// TBN matrix
	vec3 T = normalize(mat3(ubo.model) * vTangent);
	vec3 B = normalize(mat3(ubo.model) * vBitangent);
	vec3 N = normalize(mat3(ubo.model) * vNormal);
	fTBN = mat3(T, B, N);

	// gl_Position
	gl_Position = camConst.proj * camConst.view * ubo.model * vec4(vPosition, 1.0);
}