#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "gui.hpp"
#include "globals.hpp"
#include <imgui.h>
#include <iostream>

#include <glengine/loader.hpp>

void renderImGui(GLFWwindow* window, const std::vector<std::string>& objectFiles, int& currentObjectIndex, unsigned int& objectVAO, unsigned int& objectVBO, unsigned int& objectEBO, int& objectIndexCount, const char* resources_directory) {
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    ImGui::SetNextWindowPos(ImVec2(width * 0.75f, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(width * 0.25f, height), ImGuiCond_Always);

    ImGui::Begin("Rendering Parameters", nullptr, 
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoBringToFrontOnFocus
    );

    glm::vec3 camPos = orbitalCamera.getPosition();
    ImGui::Text("Camera position: (%.2f, %.2f, %.2f)", camPos.x, camPos.y, camPos.z);

    ImGui::ColorEdit3("Background Color", renderState.backgroundColor);
    ImGui::Checkbox("Show Grid", &renderState.showGrid);

    if (ImGui::CollapsingHeader("Light")) {
        const char* lighting_modes[] = { "None", "Phong", "Blinn-Phong", "Gaussian" };
        int current_mode = static_cast<int>(renderState.currentLightingMode);
        if (ImGui::Combo("Lighting Mode", &current_mode, lighting_modes, IM_ARRAYSIZE(lighting_modes))) {
            renderState.currentLightingMode = static_cast<LightingMode>(current_mode);
        }

        if (renderState.currentLightingMode != LightingMode::NONE) {
            ImGui::DragFloat3("Light Position", renderState.lightPos, 0.1f);
            ImGui::ColorEdit3("Light Color", renderState.lightColor);
            ImGui::SliderFloat("Ambient Strength", &renderState.ambientStrength, 0.0f, 1.0f);
            ImGui::SliderFloat("Specular Strength", &renderState.specularStrength, 0.0f, 1.0f);
            ImGui::SliderFloat("Shininess", &renderState.shininess, 1.0f, 256.0f);
            ImGui::Checkbox("Show Light Indicator", &renderState.showLightIndicator);
        }
    }

    if (ImGui::CollapsingHeader("Object")) {
        static int lastObjectIndex = -1;
        if (ImGui::Combo("3D Model", &currentObjectIndex, 
            [](void* data, int idx, const char** out_text) {
                std::vector<std::string>* files = (std::vector<std::string>*)data;
                if (idx < 0 || (size_t)idx >= files->size()) return false;
                *out_text = (*files)[idx].c_str();
                return true;
            }, 
            (void*)&objectFiles, objectFiles.size())) 
        {
            if (currentObjectIndex != lastObjectIndex) {
                std::string selectedObjectPath = std::string(resources_directory) + "/objects/" + objectFiles[currentObjectIndex];
                if (!reloadObject(selectedObjectPath, objectVAO, objectVBO, objectEBO, objectIndexCount)) {
                    std::cerr << "Failed to load object: " << selectedObjectPath << std::endl;
                } else {
                    lastObjectIndex = currentObjectIndex;
                }
            }
        }
        ImGui::Checkbox("Show Wireframe", &renderState.showWireframe);
        ImGui::ColorEdit3("Object Color", renderState.objectColor);
        ImGui::Checkbox("Show Normals", &renderState.showNormals);
        if (renderState.showNormals) {
            ImGui::SliderFloat("Normal Length", &renderState.normalLength, 0.01f, 1.0f);
            ImGui::ColorEdit3("Normal Color", renderState.normalColor);
        }
    }

    ImGui::End();
}

bool reloadObject(const std::string& filePath, unsigned int& VAO, unsigned int& VBO, unsigned int& EBO, int& indexCount) {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    if (!GLEngine::Loader::loadObject(filePath, vertices, indices)) {
        std::cerr << "Failed to load object: " << filePath << std::endl;
        return false;
    }

    if (VAO) glDeleteVertexArrays(1, &VAO);
    if (VBO) glDeleteBuffers(1, &VBO);
    if (EBO) glDeleteBuffers(1, &EBO);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    const int stride = (3 + 2 + 3) * sizeof(float);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    indexCount = indices.size();

    return true;
}

void setupNormalsVBO(unsigned int VAO, unsigned int VBO, int vertexCount, float normalLength, unsigned int& normalsVBO) {
    std::vector<float> normalLines;

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    float* vertexData = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
    if (vertexData == nullptr) {
        std::cerr << "Failed to map VBO for normals." << std::endl;
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        return;
    }

    // Pour chaque sommet
    const int stride = 8;
    for (int i = 0; i < vertexCount; i++) {
        float x = vertexData[i * stride + 0];
        float y = vertexData[i * stride + 1];
        float z = vertexData[i * stride + 2];

        float nx = vertexData[i * stride + 5];
        float ny = vertexData[i * stride + 6];
        float nz = vertexData[i * stride + 7];

        normalLines.push_back(x);
        normalLines.push_back(y);
        normalLines.push_back(z);

        normalLines.push_back(x + nx * normalLength);
        normalLines.push_back(y + ny * normalLength);
        normalLines.push_back(z + nz * normalLength);
    }

    glUnmapBuffer(GL_ARRAY_BUFFER);

    if (normalsVBO == 0) {
        glGenBuffers(1, &normalsVBO);
    }

    glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
    glBufferData(GL_ARRAY_BUFFER, normalLines.size() * sizeof(float), normalLines.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void setupLightIndicator(unsigned int& lightVAO, unsigned int& lightVBO, float scale = 0.2f) {
    float vertices[] = {
        -scale, -scale, -scale,
         scale, -scale, -scale,
         scale,  scale, -scale,
         scale,  scale, -scale,
        -scale,  scale, -scale,
        -scale, -scale, -scale,

        -scale, -scale,  scale,
         scale, -scale,  scale,
         scale,  scale,  scale,
         scale,  scale,  scale,
        -scale,  scale,  scale,
        -scale, -scale,  scale,

        -scale,  scale,  scale,
        -scale,  scale, -scale,
        -scale, -scale, -scale,
        -scale, -scale, -scale,
        -scale, -scale,  scale,
        -scale,  scale,  scale,

         scale,  scale,  scale,
         scale,  scale, -scale,
         scale, -scale, -scale,
         scale, -scale, -scale,
         scale, -scale,  scale,
         scale,  scale,  scale,

        -scale, -scale, -scale,
         scale, -scale, -scale,
         scale, -scale,  scale,
         scale, -scale,  scale,
        -scale, -scale,  scale,
        -scale, -scale, -scale,

        -scale,  scale, -scale,
         scale,  scale, -scale,
         scale,  scale,  scale,
         scale,  scale,  scale,
        -scale,  scale,  scale,
        -scale,  scale, -scale,
    };

    glGenVertexArrays(1, &lightVAO);
    glGenBuffers(1, &lightVBO);

    glBindVertexArray(lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}
