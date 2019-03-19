// shadertype=<hlsl>

#version 430 core

in vec3 outColor;
in vec2 TexCoord;
out vec4 color;

uniform sampler2D Txtr;
uniform sampler2D Txtr2;
uniform float currentTime;

void main()
{
    color = texture(Txtr, TexCoord);
}