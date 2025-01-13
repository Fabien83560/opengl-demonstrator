#include <glengine/computeNormal.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

namespace GLEngine {

    std::vector<float> ComputeNormal::computeNormal(const std::vector<float>& vertices, const std::vector<unsigned int>& indices) {
        std::vector<float> normals;
        for (std::size_t k = 0; k < vertices.size(); k++) {
            normals.push_back(0);
        }

        // Compute normal
        for (unsigned int k = 0; k < indices.size() - 3; k += 3) {
            unsigned int i1 = indices[k];
            unsigned int i2 = indices[k+1];
            unsigned int i3 = indices[k+2];

            // Vertices
            glm::vec3 v1 = glm::vec3(vertices[3*i1], vertices[3*i1 + 1], vertices[3*i1 + 2]);
            glm::vec3 v2 = glm::vec3(vertices[3*i2], vertices[3*i2 + 1], vertices[3*i2 + 2]);
            glm::vec3 v3 = glm::vec3(vertices[3*i3], vertices[3*i3 + 1], vertices[3*i3 + 2]);

            // Compute normal
            glm::vec3 n = glm::cross(v3 - v1, v3 - v2);

            for (std::size_t di = 0; di < 3; di++) {
                normals[3 * i1 + di] += n[di];
                normals[3 * i2 + di] += n[di];
                normals[3 * i3 + di] += n[di];
            }
        }

        // Normalize
        for (std::size_t k = 0; k < normals.size() - 3; k += 3) {
            glm::vec3 n = glm::normalize(glm::vec3(normals[k], normals[k + 1], normals[k + 2]));
            normals[k] = n[0];
            normals[k+1] = n[1];
            normals[k+2] = n[2];
        }
    }

} // namespace GLEngine
