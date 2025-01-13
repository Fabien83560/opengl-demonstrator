#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void onMouseButton(GLFWwindow* window, int button, int action, int mods);
void onMouseMove(GLFWwindow* window, double xpos, double ypos);
void onMouseScroll(GLFWwindow* window, double xoffset, double yoffset);