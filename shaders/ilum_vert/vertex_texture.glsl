#version 410

layout(location = 0) in vec4 coord;
layout(location = 1) in vec3 normal;
layout(location = 3) in vec2 texcoord;

uniform mat4 Mv; 
uniform mat4 Mn; 
uniform mat4 Mvp;

uniform vec4 lpos;  // light pos in eye space

out data {
  vec3 veye;
  vec3 neye;
  vec3 light;
  vec2 texcoord;
} v;

void main (void) 
{
  v.veye = vec3(Mv * coord);
  v.neye = normalize(vec3(Mn * vec4(normal, 0.0)));

  if (lpos.w == 0.0) 
    v.light = normalize(vec3(lpos));
  else
    v.light = normalize(vec3(lpos) - v.veye);

  v.texcoord = texcoord;

  gl_Position = Mvp * coord;
}

