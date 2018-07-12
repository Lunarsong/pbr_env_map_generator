#version 330 core

layout (location = 0) in vec3 aPosition;

out vec3 vPosition;

uniform mat4 uMatViewProjection;

void main()
{
    vPosition = aPosition;  
    gl_Position =  uMatViewProjection * vec4(aPosition, 1.0);
}