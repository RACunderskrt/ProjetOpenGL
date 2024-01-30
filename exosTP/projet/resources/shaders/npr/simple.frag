#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec3 Normal;  
in vec3 FragPos;  
  
uniform vec3 lightPos; 
uniform vec3 lightColor;
uniform vec3 cameraPos;

vec3 chooseColor(float intensity, vec3 ambient, vec3 norm){
	vec3 result;
	vec3 noir = vec3(0.0f,0.0f,0.0f);
	if (intensity > 0.7){
		result = ourColor*(1.0f-intensity)*(ambient/4);
	}
	else if (intensity > 0.3){
		if(mod(floor(gl_FragCoord.x),2.0f)== 0 && mod(floor(gl_FragCoord.y),2.0f) == 0){
			result = noir;
		}
		else
			result = ourColor*(1.0f-intensity)*(ambient/3);
	}	
	else if (intensity > 0.05){
		if(mod(floor(gl_FragCoord.x),3.0f) == 0 && mod(floor(gl_FragCoord.y),3.0f) == 0){
			result = noir;
		}
		else
			result = ourColor*(1.0f-intensity)*(ambient/2);
	}
	else
		result = ourColor*(1.0f-intensity);


	float change = abs(dFdx(norm.x) + dFdy(norm.y));
	
	if(change > 0.1){
		result = noir;
	}
	if(change > 0.07){
		result = mix(result, noir, 0.5);
	}
	if(change > 0.05){
		result = mix(result, noir, 0.3);
	}


	return result;
}


void main()
{
	float intensity;
	
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    
    float ambientStrength = 0.7;
	float specularStrength = 0.5;
    vec3 ambient = ambientStrength * lightColor;
	
	//intensity = dot(lightDir,norm);
	intensity = specularStrength * ambientStrength * dot(lightDir,norm);
	vec3 result = chooseColor(intensity, ambient, norm);
	FragColor = vec4(result, 1.0);

}

