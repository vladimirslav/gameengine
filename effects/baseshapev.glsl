#version 140
in vec3 vertexPosition_modelspace;

uniform mat4 MVP;

void main()
{
    gl_Position = MVP * vec4(vertexPosition_modelspace, 1);
}