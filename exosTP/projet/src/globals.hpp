#pragma once

#include <glengine/orbitalCamera.hpp>
#include "ShaderManager.hpp"
#include "RenderState.hpp"

extern const unsigned int SCR_WIDTH;
extern const unsigned int SCR_HEIGHT;
extern const float NEAR_PLANE;
extern const float FAR_PLANE;

extern bool firstMouse;
extern float lastX, lastY;

enum class MousePressedButton { NONE, LEFT, RIGHT, MIDDLE };
extern MousePressedButton mouseButtonState;

extern GLEngine::OrbitalCamera orbitalCamera;
extern RenderState renderState;
extern ShaderManager shaderManager;
