#version 410 core
layout(location = 0) in vec4 pos;
uniform mat4 Mvp;
out vec3 dir;

void main(void)
{
    dir = vec3(pos);
    gl_Position = Mvp*pos;
    gl_Position.z = gl_Position.w; // Z = 1.0 (Plano Distante)
}