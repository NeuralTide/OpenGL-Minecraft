#include "Camera.h"
#include "Player.h"

#include "glm/glm.hpp"
#include "GLFW/glfw3.h"

glm::vec3 position;
Camera camera;


Player::Player(glm::vec3 position) {
	this->position = position;
	this->camera = Camera(position);
	this->camera.updateCameraPosition(this->position);
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

void Player::manageMovement(GLFWwindow* window, float deltaTime) {

	const float cameraSpeed = 30.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		updatePlayerPosition(getPlayerPosition() += cameraSpeed * getCameraFront());
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		updatePlayerPosition(getPlayerPosition() -= cameraSpeed * getCameraFront());
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		updatePlayerPosition(getPlayerPosition() -= glm::normalize(glm::cross(getCameraFront(), getCameraUp())) * cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		updatePlayerPosition(getPlayerPosition() += glm::normalize(glm::cross(getCameraFront(), getCameraUp())) * cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
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