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

    // Ambient
    float ambientStrength = 0.7f;
    vec3 ambient = ambientStrength * lightColor;
    
    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Output
    vec3 totalColor = (ambient + diffuse) * objectColor;
    color = vec4(totalColor, 1.0f) * texture(Texture, TexCoord);

}