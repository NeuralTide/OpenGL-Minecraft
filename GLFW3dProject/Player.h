#ifndef PLAYER_H
#define PLAYER_H

#include "glm/glm.hpp"
#include "Camera.h"

class Player {
private:
	glm::vec3 position;
	Camera camera;

public:
	Player(glm::vec3 position = glm::vec3(0, 0, 0));
	void updatePlayerPosition(glm::vec3 newPos);
	void updateCameraRotation(double xpos, double ypos);
	glm::vec3 getPlayerPosition();
	glm::mat4 getCameraView();
	glm::vec3 getCameraFront();
	glm::vec3 getCameraUp();
	glm::vec3 getCameraPostion();


};


#endif