#include "ShaderManager.hpp"
#include <glengine/loader.hpp>
#include <iostream>

void ShaderManager::init(const char* resources_directory) {
    std::string shaderDir = std::string(resources_directory) + "/shaders/";

    shaderPrograms[LightingMode::NONE] = loadShader(
        (shaderDir + "basic_shader.vert").c_str(),
        (shaderDir + "basic_shader.frag").c_str()
    );
    if (shaderPrograms[LightingMode::NONE] == 0) {
        std::cerr << "Error: BASIC shader not initialized." << std::endl;
        return;
    }

    shaderPrograms[LightingMode::PHONG] = loadShader(
        (shaderDir + "phong_shader.vert").c_str(),
        (shaderDir + "phong_shader.frag").c_str()
    );
    if (shaderPrograms[LightingMode::PHONG] == 0) {
        std::cerr << "Error: PHONG shader not initialized." << std::endl;
        return;
    }

    shaderPrograms[LightingMode::BLINN_PHONG] = loadShader(
        (shaderDir + "blinn_phong_shader.vert").c_str(),
        (shaderDir + "blinn_phong_shader.frag").c_str()
    );
    if (shaderPrograms[LightingMode::BLINN_PHONG] == 0) {
        std::cerr << "Error: BLINN_PHONG shader not initialized." << std::endl;
        return;
    }

    shaderPrograms[LightingMode::GAUSSIAN] = loadShader(
        (shaderDir + "gaussian_shader.vert").c_str(),
        (shaderDir + "gaussian_shader.frag").c_str()
    );
    if (shaderPrograms[LightingMode::GAUSSIAN] == 0) {
        std::cerr << "Error: GAUSSIAN shader not initialized." << std::endl;
        return;
    }

    shaderPrograms[LightingMode::GRID] = loadShader(
        (shaderDir + "grid_shader.vert").c_str(),
        (shaderDir + "grid_shader.frag").c_str()
    );
    if (shaderPrograms[LightingMode::GRID] == 0) {
        std::cerr << "Error: GRID shader not initialized." << std::endl;
        return;
    }

    shaderPrograms[LightingMode::AXES] = loadShader(
        (shaderDir + "axes_shader.vert").c_str(),
        (shaderDir + "axes_shader.frag").c_str()
    );
    if (shaderPrograms[LightingMode::AXES] == 0) {
        std::cerr << "Error: AXES shader not initialized." << std::endl;
        return;
    }

    shaderPrograms[LightingMode::NORMALS] = loadShader(
        (shaderDir + "normal_shader.vert").c_str(),
        (shaderDir + "normal_shader.frag").c_str()
    );
    if (shaderPrograms[LightingMode::NORMALS] == 0) {
        std::cerr << "Error: NORMALS shader not initialized." << std::endl;
        return;
    }
}

void ShaderManager::use(LightingMode mode) {
    currentProgram = shaderPrograms[mode];
    glUseProgram(currentProgram);
}

void ShaderManager::setVec3(const std::string& name, const glm::vec3& value) {
    glUniform3fv(glGetUniformLocation(currentProgram, name.c_str()), 1, glm::value_ptr(value));
}

void ShaderManager::setFloat(const std::string& name, float value) {
    glUniform1f(glGetUniformLocation(currentProgram, name.c_str()), value);
}

void ShaderManager::setMat4(const std::string& name, const glm::mat4& value) {
    glUniformMatrix4fv(glGetUniformLocation(currentProgram, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

unsigned int ShaderManager::loadShader(const char* vertexPath, const char* fragmentPath) {
    const char* vShaderCode = GLEngine::Loader::loadShader(vertexPath);
    const char* fShaderCode = GLEngine::Loader::loadShader(fragmentPath);
    
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];
    
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    unsigned int program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    
    return program;
}