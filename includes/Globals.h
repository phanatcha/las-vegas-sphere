#pragma once
#include "camera.h"

// ~~~~~~~~~~~>constants<~~~~~~~~~~~~
namespace Config {
    constexpr unsigned int SCR_WIDTH = 800;
    constexpr unsigned int SCR_HEIGHT = 600;
}

// ~~~~~~~~~~~>shared variable<~~~~~~~~~~~~
// camera
inline Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
// timing
inline float lastFrame = 0.0f;
inline float deltaTime = 0.0f;
inline int currentPattern = 1;
//brightness
inline bool isScreenOn = true;
inline float globalBrightness = 1.0f;