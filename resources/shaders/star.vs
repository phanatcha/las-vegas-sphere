#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in float aSize; 
layout (location = 2) in float aBrightness;

uniform mat4 view;
uniform mat4 projection;

out float vBrightness;

void main()
{
    vec4 viewPos = view * vec4(aPos, 1.0);
    
    gl_Position = projection * view * vec4(aPos, 1.0);
    gl_PointSize = (aSize * 10.0) / -viewPos.z; 
    vBrightness = aBrightness;
}