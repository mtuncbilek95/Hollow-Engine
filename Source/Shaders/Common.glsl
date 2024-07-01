vec3 DecodeNormal(vec3 n)
{
    return normalize(n * 2.0 - 1.0);
}