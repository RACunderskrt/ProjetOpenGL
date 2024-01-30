#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float outlining;

void main()
{
    vec3 test =  vec3(model * vec4(aPos+normalize(aNormal)*outlining, 1.0f));
    gl_Position = projection * view * vec4(test, 1.0); 
}