#version 330 core

in vec2 UV;
out vec4 color;

uniform sampler2D sampler;
uniform vec4 colorMod;

void main()
{
    color = texture(sampler, UV).rgba * colorMod;
}