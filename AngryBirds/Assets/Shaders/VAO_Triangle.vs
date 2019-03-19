#version 430 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;

out vec3 outColor;
out vec2 TexCoord;
uniform float currentTime;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
 
void main(void)
{
	gl_Position = projection * view * model * vec4(position, 1.0);
	outColor = color;
	TexCoord = vec2(texCoord.x, 1.0 - texCoord.y);
}


