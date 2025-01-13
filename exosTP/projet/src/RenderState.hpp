#pragma once
#include <glm/glm.hpp>
#include "ShaderManager.hpp"

struct RenderState {
    // Lighting parameters
    LightingMode currentLightingMode = LightingMode::PHONG;
    float lightPos[3] = {3.0f, 3.0f, 3.0f};
    float lightColor[3] = {1.0f, 1.0f, 1.0f};
    float objectColor[3] = {0.6f, 0.6f, 0.6f};
    float backgroundColor[3] = {0.2f, 0.3f, 0.3f};
    float shininess = 32.0f;
    float ambientStrength = 0.1f;
    float specularStrength = 0.5f;
    float normalLength = 0.1f;
    float normalColor[3] = {1.0f, 1.0f, 0.0f}; 
    
    // Display options
    bool showGrid = true;
    bool showLightIndicator = false;
    bool showWireframe = false;
    bool showNormals = false;
};