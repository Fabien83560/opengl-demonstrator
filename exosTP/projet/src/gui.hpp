#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>

void renderImGui(GLFWwindow* window, const std::vector<std::string>& objectFiles, int& currentObjectIndex, unsigned int& objectVAO, unsigned int& objectVBO, unsigned int& objectEBO, int& objectIndexCount, const char* resources_directory);
bool reloadObject(const std::string& filePath, unsigned int& VAO, unsigned int& VBO, unsigned int& EBO, int& indexCount);
void setupNormalsVBO(unsigned int VAO, unsigned int VBO, int vertexCount, float normalLength, unsigned int& normalsVBO);
void setupLightIndicator(unsigned int& lightVAO, unsigned int& lightVBO, float scale);