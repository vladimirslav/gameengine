#version 330 core

in vec2 UV;
out vec4 color;

uniform sampler2D sampler;
uniform vec4 colorMod;

float a;

void main()
{
    a = 4 * texture(sampler, UV).a;
    a -= texture(sampler, UV + vec2(0.01f, 0.0f)).a;
    a -= texture(sampler, UV + vec2(-0.01f, 0.0f)).a;
    a -= texture(sampler, UV + vec2(0.0f, 0.01f)).a;
    a -= texture(sampler, UV + vec2(0.0f, -0.01f)).a;
    
    color = vec4(colorMod.r, colorMod.g, colorMod.b, a);
}