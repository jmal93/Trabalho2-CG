#version 410

in vec2 TexCoord;

uniform sampler2D tex;
uniform vec4 color;

out vec4 outcolor;

void main(void)
{
    vec4 texColor = texture(tex, TexCoord);
    outcolor = texColor * color;
}
