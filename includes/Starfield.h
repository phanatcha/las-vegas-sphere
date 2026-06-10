#pragma once
#include <vector>
#include <glm/glm.hpp>

struct Star {
    glm::vec3 position;
    float size;
    float brightness;
};

extern int MAX_STARS;
extern std::vector<Star> stars;

void initStars();