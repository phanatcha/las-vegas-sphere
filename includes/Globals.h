#pragma once
#include <camera.h>

// ~~~~~~~~~~~>constants<~~~~~~~~~~~~
namespace Config {
    constexpr unsigned int SCR_WIDTH = 800;
    constexpr unsigned int SCR_HEIGHT = 600;
}

// ~~~~~~~~~~~>shared variable<~~~~~~~~~~~~
extern Camera camera;
extern float deltaTime;

