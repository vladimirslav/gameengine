#version 140

in vec2 UV;
out vec4 color;

uniform sampler2D sampler;
uniform vec4 colorMod;

float a;

void main()
{
    a = 4 * texture(sampler, UV).a;
    a -= texture(sampler, UV + vec2(0.03f, 0.0f)).a;
    a -= texture(sampler, UV + vec2(-0.03f, 0.0f)).a;
    a -= texture(sampler, UV + vec2(0.0f, 0.03f)).a;
    a -= texture(sampler, UV + vec2(0.0f, -0.03f)).a;
    
    color = vec4(colorMod.r, colorMod.g, colorMod.b, a);
}