#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;  
in vec3 FragPos;  
  
uniform vec3 lightPos; 
uniform vec3 lightColor;
uniform vec3 cameraPos;
uniform sampler2D ourTexture;

void main()
{
    // ambient
    float ambientStrength = 0.01;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  
    
    vec4 outColor = vec4(vec3(mod(floor(4.0*TexCoord.x) + floor(4.0*TexCoord.y), 2.0)), 1.0);
    vec3 result = (ambient + diffuse + specular);
    FragColor = vec4(result, 1.0)* outColor;
} 