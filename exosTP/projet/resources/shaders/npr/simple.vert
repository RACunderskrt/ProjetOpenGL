#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 modelColor;
out vec3 FragPos;
out vec3 Normal;
out vec3 ourColor;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0); 
	ourColor = modelColor;
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;  
}