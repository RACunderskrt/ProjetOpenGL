#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

void main()
{
    FragColor = vec4(vec3(mod(floor(4.0*TexCoord.x) + floor(4.0*TexCoord.y), 2.0)), 1.0);
}