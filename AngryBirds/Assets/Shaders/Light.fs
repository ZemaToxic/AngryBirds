#version 430 core

in vec2 TexCoord;

in vec3 Normal;
in vec3 FragPos;

out vec4 color;

// texture
uniform sampler2D Texture;
uniform float currentTime;

//lighting
uniform vec3 objectColor;
uniform vec3 cameraPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float specularStrength;
uniform float ambientStrength;

void main(){

    //**ambient
    float ambientStrength = 0.7f;
    vec3 ambient = ambientStrength * lightColor;
    
    //**diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Specular
    float specularStrength = 5.0f;
    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  
        
    vec3 result = (ambient + diffuse + specular) * objectColor;
    color = vec4(result, 1.0f);
    
    //** rim lighting
    float f = 1.0 - dot(norm, viewDir);// Calculate the rim factor 
    f = smoothstep(0.0, 1.0, f);// Constrain it to the range 0 to 1 using a smoothstep  function     
    f = pow(f, 16);// Raise it to the rim exponent 
    vec3 rim =  f * vec3(1.0f, 0.0f, 0.0f) * lightColor;// Finally, multiply it by the rim  color
    
   vec3 totalColor = (ambient + diffuse + specular + rim) * objectColor;
   color = vec4(totalColor, 1.0f) * texture(Texture, TexCoord);

}