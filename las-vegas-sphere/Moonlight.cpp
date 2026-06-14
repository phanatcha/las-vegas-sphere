#include "Moonlight.h"

void sendMoonlightToShader(Shader& shader) {
    shader.setVec3("light.position", -2.0f, 3.0f, -3.0f); 
    shader.setVec3("light.ambient", 0.02f, 0.02f, 0.05f);
    shader.setVec3("light.diffuse", 0.2f, 0.2f, 0.3f);
    shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
}
