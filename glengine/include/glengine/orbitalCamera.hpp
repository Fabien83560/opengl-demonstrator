#ifndef ORBITAL_CAMERA_HPP
#define ORBITAL_CAMERA_HPP

#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>

//
//                  |
//                 YAW
//                  |
//     R         _  |
//     O        _\_\|____/ )
//   --L------(0        __/
//     L        --\ \----
//                 \ \
//                   -
//                    \
//                    PITCH
//                      \
//

namespace GLEngine {
	class OrbitalCamera {
	public:
        enum class Movement {
            BACK,
            FORWARD,
            YAW,
            PITCH
        };

        OrbitalCamera(glm::vec3 _position = glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3 _focus = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 _up = glm::vec3(0.0f, -1.0f, 0.0f));
        glm::mat4 getViewMatrix();
        glm::vec3 getPosition() const;
        float getFov() const;
        void orbit(float xoffset, float yoffset);
        void dolly(float offset);
        void track(float offset);
        void pedestal(float offset);
        void zoom(float offset);

    private:
        glm::vec3 position;
        glm::vec3 focus;
        glm::vec3 up;

        float fov;
        float yaw;
        float pitch;
        glm::vec3 right;
        glm::vec3 camFocusVector;

        void updateCameraVectors();
	};
}
#endif
