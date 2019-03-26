#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normals;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 vp;
uniform mat4 model;

void main(void)
{
	gl_Position = vp * model *vec4(position, 1.0);
	TexCoord = vec2(texCoord.x, 1- texCoord.y);
	FragPos = vec3(model* vec4(position, 1.0f));
	Normal = mat3(transpose(inverse(model))) * normals;
}