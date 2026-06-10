#version 410 core
out vec4 FragColor;

in vec3 localPos;

void main()
{
    // The sphere's radius is 1.0. 
    // The equator is at Y = 0.0.
    // The bottom is at Y = -1.0.
    if (localPos.y < -0.3) {
        discard;
    }
    FragColor = vec4(1.0);
}