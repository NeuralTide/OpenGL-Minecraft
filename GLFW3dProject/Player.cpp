#include "Camera.h"
#include "Player.h"

#include "glm/glm.hpp"

glm::vec3 position;
Camera camera;


Player::Player(glm::vec3 position) {
	this->position = position;
	this->camera = Camera(position);
}

glm::vec3 Player::getPlayerPosition() {

	return this->position;
}

void Player::updatePlayerPosition(glm::vec3 newPos) {
	this->position = newPos;
	camera.updateCameraPosition(position);
}

glm::vec3 Player::getCameraFront() {
	return camera.getFront();
}

void Player::updateCameraRotation(double xpos, double ypos) {
	camera.updateCameraRotation(xpos, ypos);
}

glm::mat4 Player::getCameraView() {

	return camera.getView();
}

glm::vec3 Player::getCameraPostion() {
	return camera.getPosition();
}

glm::vec3 Player::getCameraUp() {
	return camera.getUp();
}