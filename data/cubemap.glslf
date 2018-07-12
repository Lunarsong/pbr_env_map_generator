#version 330 core

uniform samplerCube sampler0;

in vec3 vUV;
out vec4 outColor;
void main()
{
    vec3 color = texture(sampler0, vUV).rgb;
    
    outColor = vec4(color, 1.0);
}
