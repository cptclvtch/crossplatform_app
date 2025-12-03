#version 310 es

uniform vec3 color;

out vec4 glFragColor;

void main()
{
    glFragColor = vec4(color, 0.5);
}