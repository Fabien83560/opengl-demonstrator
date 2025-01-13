#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <unordered_map>

enum class LightingMode {
    NONE,
    PHONG,
    BLINN_PHONG,
    GAUSSIAN,
    GRID,
    AXES,
    NORMALS
};

class ShaderManager {
public:
    void init(const char* resources_directory);
    void use(LightingMode mode);
    void setVec3(const std::string& name, const glm::vec3& value);
    void setFloat(const std::string& name, float value);
    void setMat4(const std::string& name, const glm::mat4& value);
    unsigned int getCurrentProgram() const { return currentProgram; }

private:
    unsigned int loadShader(const char* vertexPath, const char* fragmentPath);
    std::unordered_map<LightingMode, unsigned int> shaderPrograms;
    unsigned int currentProgram = 0;
};