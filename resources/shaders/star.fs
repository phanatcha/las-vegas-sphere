#version 410 core

in float vBrightness;

out vec4 FragColor;

void main()
{
    vec2 coord = gl_PointCoord - vec2(0.5);
    
    float dist = length(coord);
    
    if (dist > 0.5) {
        discard;
    }

    float glow = 1.0 - (dist * 2.0);
    vec3 starColor = vec3(1.0, 1.0, 1.0);
    vec3 finalColor = starColor * glow * vBrightness;
    
    FragColor = vec4(finalColor, 1.0);
}