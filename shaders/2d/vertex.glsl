#version 410

layout(location = 0) in vec3 vertex;
layout(location = 2) in vec2 texcoord;

uniform mat4 Mvp;

out vec2 TexCoord;

void main(void)
{
    gl_Position = Mvp * vec4(vertex, 1.0);
    TexCoord = texcoord;
}
