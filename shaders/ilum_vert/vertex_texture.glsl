#version 410 core

layout(location = 0) in vec4 coord;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 tangent;
layout(location = 3) in vec2 texcoord;

uniform mat4 Mv;
uniform mat4 Mn;
uniform mat4 Mvp;
uniform vec4 lpos;

out data {
    vec3 vEye;
    vec2 vTexcoord;
    mat3 TBN;
    vec3 lightEye;
} v;

void main(void)
{
    vec3 posEye = vec3(Mv * coord);
    v.vEye = posEye;

    vec3 N = normalize(vec3(Mn * vec4(normal, 0.0)));
    vec3 T = normalize(vec3(Mn * vec4(tangent, 0.0)));
    vec3 B = normalize(cross(T, N));
    v.TBN = mat3(T, B, N);

    v.vTexcoord = texcoord;

    vec3 lightDirEye;
    if (lpos.w == 0.0)
        lightDirEye = normalize(vec3(lpos));
    else
        lightDirEye = normalize(vec3(lpos) - posEye);
    v.lightEye = lightDirEye;

    gl_Position = Mvp * coord;
}
