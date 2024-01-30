#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 ourNormal;
out vec2 TexCoord;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    ourNormal = aNormal;
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}