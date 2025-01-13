#include "grid.hpp"
#include <glad/glad.h>
#include <vector>

void setupGrid(unsigned int& gridVAO, unsigned int& gridVBO, std::vector<float>& gridVertices) {
    const float gridSize = 10.0f;
    const float gridStep = 1.0f;

    for (float i = -gridSize; i <= gridSize; i += gridStep) {
        gridVertices.push_back(i);
        gridVertices.push_back(0.0f);
        gridVertices.push_back(-gridSize);
        gridVertices.push_back(0.5f);
        gridVertices.push_back(0.5f);
        gridVertices.push_back(0.5f);

        gridVertices.push_back(i);
        gridVertices.push_back(0.0f);
        gridVertices.push_back(gridSize);
        gridVertices.push_back(0.5f);
        gridVertices.push_back(0.5f);
        gridVertices.push_back(0.5f);

        // Horizontal lines (Z constant)
        gridVertices.push_back(-gridSize);
        gridVertices.push_back(0.0f);
        gridVertices.push_back(i);
        gridVertices.push_back(0.5f);
        gridVertices.push_back(0.5f);
        gridVertices.push_back(0.5f);

        gridVertices.push_back(gridSize);
        gridVertices.push_back(0.0f);
        gridVertices.push_back(i);
        gridVertices.push_back(0.5f);
        gridVertices.push_back(0.5f);
        gridVertices.push_back(0.5f);
    }

    glGenVertexArrays(1, &gridVAO);
    glGenBuffers(1, &gridVBO);

    glBindVertexArray(gridVAO);
    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
    glBufferData(GL_ARRAY_BUFFER, gridVertices.size() * sizeof(float), gridVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void setupAxes(unsigned int& axesVAO, unsigned int& axesVBO) {
    const float gridSize = 10.0f;
    std::vector<float> axesVertices = {
        // X axis: Red
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        gridSize, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

        // Y axis: Green
        0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, gridSize, 0.0f, 0.0f, 1.0f, 0.0f,

        // Z axis: Blue
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, gridSize, 0.0f, 0.0f, 1.0f
    };

    glGenVertexArrays(1, &axesVAO);
    glGenBuffers(1, &axesVBO);

    glBindVertexArray(axesVAO);
    glBindBuffer(GL_ARRAY_BUFFER, axesVBO);
    glBufferData(GL_ARRAY_BUFFER, axesVertices.size() * sizeof(float), axesVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
