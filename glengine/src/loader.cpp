#include <glengine/loader.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <glm/glm.hpp>

namespace GLEngine {

    char* Loader::loadShader(const std::string& filePath) {
        std::ifstream shaderFile(filePath);
        if (!shaderFile.is_open()) {
            std::cerr << "Failed to open shader file: " << filePath << std::endl;
            return nullptr;
        }

        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();

        std::string shaderCode = shaderStream.str();
        char* shaderCodeCStr = new char[shaderCode.size() + 1];
        std::copy(shaderCode.begin(), shaderCode.end(), shaderCodeCStr);
        shaderCodeCStr[shaderCode.size()] = '\0';

        return shaderCodeCStr;
    }

    bool Loader::loadObject(const std::string& filePath, std::vector<float>& vertices, std::vector<unsigned int>& indices) {
        std::vector<glm::vec3> temp_positions;
        std::vector<glm::vec3> temp_normals;
        std::vector<glm::vec2> temp_texCoords;

        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "Could not open the file: " << filePath << std::endl;
            return false;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream stream(line);
            std::string prefix;
            stream >> prefix;

            if (prefix == "v") {
                glm::vec3 position;
                stream >> position.x >> position.y >> position.z;
                temp_positions.push_back(position);
            } else if (prefix == "vt") {
                glm::vec2 texCoord;
                stream >> texCoord.x >> texCoord.y;
                temp_texCoords.push_back(texCoord);
            } else if (prefix == "vn") {
                glm::vec3 normal;
                stream >> normal.x >> normal.y >> normal.z;
                float length = glm::length(normal);
                if (length > 0) {
                    normal = glm::normalize(normal);
                }
                temp_normals.push_back(normal);
            } else if (prefix == "f") {
                std::vector<unsigned int> posIndices;
                std::vector<unsigned int> texIndices;
                std::vector<unsigned int> normIndices;

                std::string vertexData;
                while (stream >> vertexData) {
                    std::istringstream vertexStream(vertexData);
                    std::string posStr, texStr, normStr;
                    std::getline(vertexStream, posStr, '/');
                    std::getline(vertexStream, texStr, '/');
                    std::getline(vertexStream, normStr, '/');

                    unsigned int posIndex = std::stoi(posStr);
                    unsigned int texIndex = texStr.empty() ? 1 : std::stoi(texStr);
                    unsigned int normIndex = normStr.empty() ? 1 : std::stoi(normStr);

                    if (posIndex == 0 || posIndex > temp_positions.size()) {
                        std::cerr << "Invalid position index in face: " << line << std::endl;
                        continue;
                    }

                    posIndices.push_back(posIndex - 1);
                    texIndices.push_back(texIndex - 1);
                    normIndices.push_back(normIndex - 1);
                }

                // Triangulation
                for (size_t i = 1; i + 1 < posIndices.size(); ++i) {
                    // Vertex 1
                    glm::vec3 pos = temp_positions[posIndices[0]];
                    vertices.push_back(pos.x);
                    vertices.push_back(pos.y);
                    vertices.push_back(pos.z);

                    glm::vec2 tex = texIndices[0] < temp_texCoords.size() ? temp_texCoords[texIndices[0]] : glm::vec2(0.0f);
                    vertices.push_back(tex.x);
                    vertices.push_back(tex.y);

                    glm::vec3 norm = normIndices[0] < temp_normals.size() ? temp_normals[normIndices[0]] : glm::vec3(0.0f, 1.0f, 0.0f);
                    vertices.push_back(norm.x);
                    vertices.push_back(norm.y);
                    vertices.push_back(norm.z);

                    // Vertex 2
                    pos = temp_positions[posIndices[i]];
                    vertices.push_back(pos.x);
                    vertices.push_back(pos.y);
                    vertices.push_back(pos.z);

                    tex = texIndices[i] < temp_texCoords.size() ? temp_texCoords[texIndices[i]] : glm::vec2(0.0f);
                    vertices.push_back(tex.x);
                    vertices.push_back(tex.y);

                    norm = normIndices[i] < temp_normals.size() ? temp_normals[normIndices[i]] : glm::vec3(0.0f, 1.0f, 0.0f);
                    vertices.push_back(norm.x);
                    vertices.push_back(norm.y);
                    vertices.push_back(norm.z);

                    // Vertex 3
                    pos = temp_positions[posIndices[i + 1]];
                    vertices.push_back(pos.x);
                    vertices.push_back(pos.y);
                    vertices.push_back(pos.z);

                    tex = texIndices[i + 1] < temp_texCoords.size() ? temp_texCoords[texIndices[i + 1]] : glm::vec2(0.0f);
                    vertices.push_back(tex.x);
                    vertices.push_back(tex.y);

                    norm = normIndices[i + 1] < temp_normals.size() ? temp_normals[normIndices[i + 1]] : glm::vec3(0.0f, 1.0f, 0.0f);
                    vertices.push_back(norm.x);
                    vertices.push_back(norm.y);
                    vertices.push_back(norm.z);

                    indices.push_back(indices.size());
                    indices.push_back(indices.size());
                    indices.push_back(indices.size());
                }
            }
        }

        file.close();
        return true;
    }
} // namespace GLEngine
