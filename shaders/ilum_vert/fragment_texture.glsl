#version 410 core

in data {
    vec3 vEye;
    vec2 vTexcoord;
    mat3 TBN;
    vec3 lightEye;
} f;

out vec4 color;

uniform sampler2D decal;
uniform sampler2D normalMap;

uniform vec4 lamb;
uniform vec4 ldif;
uniform vec4 lspe;

uniform vec4 mamb;
uniform vec4 mdif;
uniform vec4 mspe;
uniform float mshi;

void main(void)
{
    vec3 normalTex = texture(normalMap, f.vTexcoord).rgb;
    normalTex = normalTex * 2.0 - 1.0;

    vec3 N = normalize(f.TBN * normalTex);

    vec3 L = normalize(f.lightEye);
    vec3 V = normalize(-f.vEye);

    vec4 ambient = mamb * lamb;

    float ndotl = max(dot(N, L), 0.0);
    vec4 diffuse = ndotl * mdif * ldif;

    vec4 specular = vec4(0.0);
    if (ndotl > 0.0) {
        vec3 R = reflect(-L, N);
        float rdotv = max(dot(R, V), 0.0);
        specular = mspe * lspe * pow(rdotv, mshi);
    }

    vec4 texColor = texture(decal, f.vTexcoord);
    color = (ambient + diffuse + specular) * texColor;
}
