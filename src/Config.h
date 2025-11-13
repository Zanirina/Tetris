// Config.h
#pragma once
#include <glm/glm.hpp>

namespace Config {
    const int GRID_WIDTH = 10;
    const int GRID_HEIGHT = 20;
    const float FALL_INTERVAL = 0.7f; // скорость падения
    const glm::vec3 COLORS[] = {
        {1.0f,0.3f,0.3f},
        {0.3f,1.0f,0.3f},
        {0.3f,0.3f,1.0f},
        {1.0f,1.0f,0.3f},
        {1.0f,0.3f,1.0f},
        {0.3f,1.0f,1.0f},
        {1.0f,0.5f,0.2f}
    };
}
