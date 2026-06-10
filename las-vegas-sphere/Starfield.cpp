#include "Starfield.h"
#include <random>

int MAX_STARS = 10000;
std::vector<Star> stars;

void initStars() {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<float> posRange(-100.0f, 100.0f);
    std::uniform_real_distribution<float> sizeRange(1.0f, 6.0f);
    std::uniform_real_distribution<float> brightRange(0.0f, 1.0f);

    for (int i = 0; i< MAX_STARS; i++)
    {
        Star s;

        s.position = glm::vec3(
            posRange(gen),
            posRange(gen),
            posRange(gen)
        );

        s.size = sizeRange(gen);
        s.brightness = brightRange(gen);

        stars.push_back(s);
    }
}