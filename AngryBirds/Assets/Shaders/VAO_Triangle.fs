#version 430 core

in vec3 outColor;
in vec2 TexCoord;

out vec4 color;

uniform float currentTime;
uniform sampler2D Texture;
uniform sampler2D Texture2;

void main()
{
    // color = mix(texture(Texture, TexCoord), texture(Texture2, TexCoord), 0.8);

    color = vec4(1.0f, 1.0f, 0.0f, 1.0f);
}