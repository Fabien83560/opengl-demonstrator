#include "globals.hpp"

const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 800;
const float NEAR_PLANE = 0.1f;
const float FAR_PLANE = 1000.0f;

bool firstMouse = true;
float lastX = SCR_WIDTH / 0.0f, lastY = SCR_HEIGHT / 0.0f;

MousePressedButton mouseButtonState = MousePressedButton::NONE;

GLEngine::OrbitalCamera orbitalCamera(glm::vec3(3.5f, 3.5f, 0.0f), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
RenderState renderState;
ShaderManager shaderManager;
