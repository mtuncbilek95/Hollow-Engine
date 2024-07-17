const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH2 = max(dot(N, H), 0.0);
    NdotH2 = NdotH2 * NdotH2;

    float sub = (NdotH2 * (a2 - 1.0) + 1.0);
    return a2 / (PI * sub * sub);
};

// NdotX stands for dot product of normal and some other vector 
// because it's used in many places
float GeometrySchlickGGX(float NdotX, float roughness)
{
    float r = (roughness + 1.0); // constant that dependent to roughness
    float k = (r * r) / 8.0; // constant that dependent to roughness

    float sub = NdotX * (1.0 - k) + k; // denominator of the formula

    return NdotX / sub;   
};

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggxNV = GeometrySchlickGGX(NdotV, roughness);
    float ggxNL = GeometrySchlickGGX(NdotL, roughness);

    return ggxNV * ggxNL;
};

vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
};