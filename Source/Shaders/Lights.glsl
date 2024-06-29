struct DiffuseLight
{
	vec4 Position;
	vec4 Color;
	float Intensity;
};

struct AmbientLight
{
	vec4 Color;
	float Intensity;
};

vec4 GetAmbientLight(AmbientLight light)
{
	return light.Color * light.Intensity;
};

vec3 GetDiffuseLight(DiffuseLight light, vec3 normal, vec3 position)
{
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(light.Position.xyz - position);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.Color.rgb * diff * light.Intensity;

	return diffuse;
};