#version 410

in data {
  vec3 veye;
  vec3 neye;
  vec3 light;
  vec2 texcoord;
} f;

out vec4 color;

uniform sampler2D decal;

uniform vec4 lamb;
uniform vec4 ldif;
uniform vec4 lspe;

uniform vec4 mamb;
uniform vec4 mdif;
uniform vec4 mspe;
uniform float mshi;

void main (void)
{
  vec3 N = normalize(f.neye);
  vec3 L = normalize(f.light);
  vec3 V = normalize(-f.veye);

  vec4 ambient = mamb * lamb;

  float ndotl = max(dot(N,L), 0.0);
  vec4 diffuse = ndotl * mdif * ldif;

  vec4 specular = vec4(0.0);
  if (ndotl > 0.0) {
    vec3 R = reflect(-L, N);
    float rdotv = max(dot(R, V), 0.0);
    specular = mspe * lspe * pow(rdotv, mshi);
  }

  vec4 lighting = ambient + diffuse + specular;

  vec4 texColor = texture(decal, f.texcoord);
  color = lighting * texColor;
}

