#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <filesystem>
#include <iostream>
#include <vector>

#include "globals.hpp"
#include "gui.hpp"
#include "ShaderManager.hpp"
#include "RenderState.hpp"
#include "callbacks.hpp"
#include <glengine/orbitalCamera.hpp>
#include <glengine/loader.hpp>

#include "projet/config.hpp"

namespace fs = std::filesystem;

// Function declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void onMouseButton(GLFWwindow* window, int button, int action, int mods);
void onMouseMove(GLFWwindow* window, double xpos, double ypos);
void onMouseScroll(GLFWwindow* window, double xoffset, double yoffset);
void setupGrid(unsigned int& gridVAO, unsigned int& gridVBO, std::vector<float>& gridVertices);
void setupAxes(unsigned int& axesVAO, unsigned int& axesVBO);

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Viewer", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Setup callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, onMouseButton);
    glfwSetCursorPosCallback(window, onMouseMove);
    glfwSetScrollCallback(window, onMouseScroll);

    // ImGui initialization
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    shaderManager.init(_resources_directory);

    glEnable(GL_DEPTH_TEST);

    unsigned int gridVAO, gridVBO, axesVAO, axesVBO;
    unsigned int normalsVBO = 0;
    unsigned int lightVAO = 0, lightVBO = 0;
    std::vector<float> gridVertices;
    setupGrid(gridVAO, gridVBO, gridVertices);
    setupAxes(axesVAO, axesVBO);
    setupLightIndicator(lightVAO, lightVBO, 0.5f);

    // Load available objects
    std::vector<std::string> objectFiles;
    for (const auto& entry : fs::directory_iterator(std::string(_resources_directory) + "/objects")) {
        if (entry.path().extension() == ".obj") {
            objectFiles.push_back(entry.path().filename().string());
        }
    }

    unsigned int objectVAO = 0, objectVBO = 0, objectEBO = 0;
    int objectIndexCount = 0;

    // Load default object
    if (!objectFiles.empty()) {
        std::string firstObjectPath = std::string(_resources_directory) + "/objects/" + objectFiles[0];
        if (!reloadObject(firstObjectPath, objectVAO, objectVBO, objectEBO, objectIndexCount)) {
            std::cerr << "Failed to load the default object." << std::endl;
        }
    }

    static int currentObjectIndex = 0;
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        glClearColor(renderState.backgroundColor[0], renderState.backgroundColor[1], renderState.backgroundColor[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = orbitalCamera.getViewMatrix();
        glm::mat4 projection = glm::perspective(orbitalCamera.getFov(), (float)SCR_WIDTH / (float)SCR_HEIGHT, NEAR_PLANE, FAR_PLANE);

        // Draw grid if enabled
        if (renderState.showGrid) {
            shaderManager.use(LightingMode::GRID);
            shaderManager.setMat4("view", view);
            shaderManager.setMat4("projection", projection);
            shaderManager.setMat4("model", model);
            shaderManager.setVec3("gridColor", glm::vec3(0.5f, 0.5f, 0.5f));

            glBindVertexArray(gridVAO);
            glDrawArrays(GL_LINES, 0, gridVertices.size() / 6);
            glBindVertexArray(0);

            shaderManager.use(LightingMode::AXES);
            shaderManager.setMat4("view", view);
            shaderManager.setMat4("projection", projection);
            shaderManager.setMat4("model", model);

            glBindVertexArray(axesVAO);
            glDrawArrays(GL_LINES, 0, 6);
            glBindVertexArray(0);
        }

        shaderManager.use(renderState.currentLightingMode);
        shaderManager.setMat4("model", model);
        shaderManager.setMat4("view", view);
        shaderManager.setMat4("projection", projection);

        if (renderState.currentLightingMode != LightingMode::NONE) {
            shaderManager.setVec3("lightPos", glm::vec3(renderState.lightPos[0], renderState.lightPos[1], renderState.lightPos[2]));
            shaderManager.setVec3("viewPos", orbitalCamera.getPosition());
            shaderManager.setVec3("lightColor", glm::vec3(renderState.lightColor[0], renderState.lightColor[1], renderState.lightColor[2]));
            shaderManager.setVec3("objectColor", glm::vec3(renderState.objectColor[0], renderState.objectColor[1], renderState.objectColor[2]));
            shaderManager.setFloat("shininess", renderState.shininess);
            shaderManager.setFloat("ambientStrength", renderState.ambientStrength);
            shaderManager.setFloat("specularStrength", renderState.specularStrength);
        }

        // Draw object
        glPolygonMode(GL_FRONT_AND_BACK, renderState.showWireframe ? GL_LINE : GL_FILL);
        
        if (objectVAO && objectIndexCount > 0) {
            shaderManager.use(renderState.currentLightingMode);
            shaderManager.setMat4("model", model);
            shaderManager.setMat4("view", view);
            shaderManager.setMat4("projection", projection);

            if (renderState.currentLightingMode != LightingMode::NONE) {
                shaderManager.setVec3("lightPos", glm::vec3(renderState.lightPos[0], renderState.lightPos[1], renderState.lightPos[2]));
                shaderManager.setVec3("viewPos", orbitalCamera.getPosition());
                shaderManager.setVec3("lightColor", glm::vec3(renderState.lightColor[0], renderState.lightColor[1], renderState.lightColor[2]));
                shaderManager.setVec3("objectColor", glm::vec3(renderState.objectColor[0], renderState.objectColor[1], renderState.objectColor[2]));
                shaderManager.setFloat("shininess", renderState.shininess);
                shaderManager.setFloat("ambientStrength", renderState.ambientStrength);
                shaderManager.setFloat("specularStrength", renderState.specularStrength);
            }

            glBindVertexArray(objectVAO);
            glDrawElements(GL_TRIANGLES, objectIndexCount, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            if (renderState.showNormals) {
                setupNormalsVBO(objectVAO, objectVBO, objectIndexCount / 3, renderState.normalLength, normalsVBO);

                shaderManager.use(LightingMode::NORMALS);
                shaderManager.setMat4("model", model);
                shaderManager.setMat4("view", view);
                shaderManager.setMat4("projection", projection);
                shaderManager.setVec3("normalColor", glm::vec3(renderState.normalColor[0], renderState.normalColor[1], renderState.normalColor[2]));

                unsigned int normalVAO;
                glGenVertexArrays(1, &normalVAO);
                glBindVertexArray(normalVAO);
                
                glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
                glEnableVertexAttribArray(0);

                glDrawArrays(GL_LINES, 0, objectIndexCount * 2);

                glDeleteVertexArrays(1, &normalVAO);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glBindVertexArray(0);
            }
        }

        if (renderState.showLightIndicator && renderState.currentLightingMode != LightingMode::NONE) {
            shaderManager.use(LightingMode::NONE);
            glm::mat4 lightModel = glm::mat4(1.0f);
            lightModel = glm::translate(lightModel, glm::vec3(renderState.lightPos[0], renderState.lightPos[1], renderState.lightPos[2]));
            lightModel = glm::scale(lightModel, glm::vec3(0.4f));
            shaderManager.setMat4("model", lightModel);
            shaderManager.setMat4("view", view);
            shaderManager.setMat4("projection", projection);
            shaderManager.setVec3("objectColor", glm::vec3(renderState.lightColor[0], renderState.lightColor[1], renderState.lightColor[2]));

            glBindVertexArray(lightVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
        }

        renderImGui(window, objectFiles, currentObjectIndex, objectVAO, objectVBO, objectEBO, objectIndexCount, _resources_directory);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    if (normalsVBO) glDeleteBuffers(1, &normalsVBO);
    if (objectVAO) glDeleteVertexArrays(1, &objectVAO);
    if (objectVBO) glDeleteBuffers(1, &objectVBO);
    if (objectEBO) glDeleteBuffers(1, &objectEBO);
    if (lightVAO) glDeleteVertexArrays(1, &lightVAO);
    if (lightVBO) glDeleteBuffers(1, &lightVBO);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();

    return 0;
}
