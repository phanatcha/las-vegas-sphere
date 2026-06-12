#version 410 core
out vec4 FragColor;

in vec3 localPos;
uniform float time;

uniform int patternType;

void main()
{
    // The sphere's radius is 1.0. 
    // The equator is at Y = 0.0.
    // The bottom is at Y = -1.0.
    if (localPos.y < -0.3) {
        discard;
    }

    vec3 color = vec3(0.0);
    float dist = distance(localPos, vec3(0.0, 1.0, 0.0));

    switch (patternType) {
        case 1: // Neon Plasma
            float plasma = sin(localPos.x * 8.0 + time * 2.0) +
                                      sin(localPos.y * 8.0 - time * 1.5) +
                                      sin(localPos.z * 8.0 + time * 3.0);

            float glow = smoothstep(-1.0, 1.0, plasma);

            color = mix(vec3(0.1, 0.0, 0.2), vec3(0.0, 1.0, 0.8), glow);
            break;

        case 2: // Radar Scanner
            float radarAngle = atan(localPos.z, localPos.x);
            float sweep = fract((radarAngle / 6.2831) + time * 0.5);
            float scanline = smoothstep(0.8, 1.0, sweep) + smoothstep(0.0, 0.05, 1.0 - sweep) * 0.5;
            color = mix(vec3(0.0, 0.1, 0.0), vec3(0.2, 1.0, 0.4), scanline);
            break;

        case 3: // Equator Pulse
            float bands = abs(sin(localPos.y * 20.0 - time * 5.0));
            float strobe = pow(bands, 4.0);
            float equatorFade = smoothstep(0.8, 0.0, abs(localPos.y));
            color = mix(vec3(0.1, 0.0, 0.0), vec3(1.0, 0.3, 0.1), strobe * equatorFade);
            break;

        case 4: // digital cyber-grid
            vec3 scaledPos = localPos * 12.0;
            vec3 grid = abs(fract(scaledPos) - 0.5);

            float lineX = smoothstep(0.4, 0.45, grid.x);
            float lineY = smoothstep(0.4, 0.45, grid.y);
            float lineZ = smoothstep(0.4, 0.45, grid.z);

            float gridMask = clamp(lineX + lineY + lineZ, 0.0, 1.0);
            float pulse = (sin(time * 3.0) + 1.0) * 0.5;

            color = mix(vec3(0.0, 0.1, 0.0), vec3(0.0, 1.0, 0.3), gridMask * pulse);
            break;

        case 5: // organic gyriod magma
            vec3 p = localPos * 10.0 + vec3(time, time * 0.5, 0.0);
            float gyroid = abs(dot(sin(p), cos(vec3(p.z, p.x, p.y))));
            float wave5 = smoothstep(0.0, 1.5, gyroid);
            color = mix(vec3(0.8, 0.1, 0.0), vec3(1.0, 0.9, 0.2), wave5);
            break;

        default: 
            color = vec3(1.0);
    }

    FragColor = vec4(color, 1.0);
}