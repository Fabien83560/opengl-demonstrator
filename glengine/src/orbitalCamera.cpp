#include <glengine/orbitalCamera.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <iostream>

namespace GLEngine {
	OrbitalCamera::OrbitalCamera(glm::vec3 _position, glm::vec3 _focus, glm::vec3 _up)
	: position(_position), focus(_focus), up(_up), fov(45.0f), yaw(0.0f), pitch(0.0f) {
		updateCameraVectors();
	}

	glm::mat4 OrbitalCamera::getViewMatrix() {
		return glm::lookAt(position, focus, up);
	}

	glm::vec3 OrbitalCamera::getPosition() const {
		return position;
	}

	float OrbitalCamera::getFov() const {
		return glm::radians(fov);
	}

	void OrbitalCamera::orbit(float xoffset, float yoffset) {
		const float epsilon = 0.1f;
		xoffset *= 0.2f;
		yoffset *= 0.2f;

		float a = glm::degrees(glm::angle(glm::normalize(position - focus), up));

		if (a + yoffset < epsilon || a + yoffset > 180 - epsilon)
			yoffset = 0.0;

		yaw -= xoffset;
		pitch -= yoffset;

		updateCameraVectors();
		yaw = 0.0f;
		pitch = 0.0;
	}

	void OrbitalCamera::dolly(float offset) {
		const float epsilon = 0.005f;
		offset *= 0.1f;

		glm::vec3 cfv = focus - position;
		if (glm::length(cfv) - offset < epsilon)
			offset = 0.0f;

		position = focus - (cfv - offset * glm::normalize(cfv));
		updateCameraVectors();
	}

	void OrbitalCamera::track(float offset) {
		offset *= 0.01f;

		glm::vec3 dRight = offset * right;
		position += dRight;
		focus += dRight;
		updateCameraVectors();
	}

	void OrbitalCamera::pedestal(float offset) {
		offset *= 0.01f;

		glm::vec3 dUp = offset * up;
		position -= dUp;
		focus -= dUp;
		updateCameraVectors();
	}

	void OrbitalCamera::zoom(float offset) {
		fov -= (float)offset;

		if (fov < 1.0f)
			fov = 1.0f;

		if (fov > 100.0f)
			fov = 100.0f;
	}

	void OrbitalCamera::updateCameraVectors() {
		glm::vec3 cfv = focus - position;
		right = glm::normalize(-glm::cross(cfv, up));
		
		cfv = glm::rotate(cfv, glm::radians(yaw), up);
		cfv = glm::rotate(cfv, glm::radians(pitch), right);

		position = focus - cfv;
	}
}
