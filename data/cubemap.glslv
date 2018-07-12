#version 330 core
layout (location = 0) in vec3 aPosition;

out vec3 vUV;

uniform mat4 uMatViewProjection;

void main()
{
    vUV = aPosition.xyz;
    gl_Position =  uMatViewProjection * vec4(aPosition, 1.0).xyww;
}
