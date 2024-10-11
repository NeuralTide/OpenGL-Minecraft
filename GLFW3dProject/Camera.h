#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
private:
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;

	bool firstMouse;
	float yaw;
	float pitch;
	float lastX;
	float lastY;
	float fov;

public:
	void updateCameraPosition(glm::vec3 pos);
	void updateCameraRotation(double xpos, double ypos);

	glm::mat4 getView();	
	glm::vec3 getFront();
	glm::vec3 getUp();
	glm::vec3 getPosition();
	
	Camera(glm::vec3 position = glm::vec3(0, 0, 0));

};

#endif