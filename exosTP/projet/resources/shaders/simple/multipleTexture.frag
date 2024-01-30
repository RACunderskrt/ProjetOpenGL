#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D texture4;

void main()
{
    FragColor = texture(texture1, TexCoord) * texture(texture2, TexCoord) + texture(texture3, TexCoord) * texture(texture4, TexCoord) * vec4(1.0f, 0.5f, 0.2f, 1.0f);
}