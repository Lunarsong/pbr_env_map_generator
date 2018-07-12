#version 330 core

uniform sampler2D sampler0;

const vec2 kInverseAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= kInverseAtan;
    uv += 0.5;
    return uv;
}

in vec3 vPosition;
out vec4 outColor;
void main()
{		
    vec2 uv = SampleSphericalMap(normalize(vPosition));
    vec3 color = texture(sampler0, uv).rgb;
    
    outColor = vec4(color, 1.0);
}
