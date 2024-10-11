
#include "Camera.h"
#include "glm/glm.hpp"


//camera stuff
glm::vec3 cameraPos;
glm::vec3 cameraFront;
glm::vec3 cameraUp;

bool firstMouse;
float yaw;
float pitch;
float lastX;
float lastY;
float fov;


void Camera::updateCameraRotation(double xpos, double ypos) 
{
    if (this->firstMouse)
    {
        this->lastX = xpos;
        this->lastY = ypos;
        this->firstMouse = false;
    }

    float xoffset = xpos - this->lastX;
    float yoffset = this->lastY - ypos;
    this->lastX = xpos;
    this->lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    this->yaw += xoffset;
    this->pitch += yoffset;

    if (this->pitch > 89.0f)
        this->pitch = 89.0f;
    if (this->pitch < -89.0f)
        this->pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    this->cameraFront = glm::normalize(front);
}

void Camera::updateCameraPosition(glm::vec3 pos) {
    this->cameraPos = pos;
}

glm::vec3 Camera::getFront() {
    return this->cameraFront;
}

glm::vec3 Camera::getUp() {
    return this->cameraUp;
}

glm::vec3 Camera::getPosition() {
    return this->cameraPos;
}

glm::mat4 Camera::getView() {
    return glm::lookAt(this->cameraPos, this->cameraPos + this->cameraFront, this->cameraUp);
}

Camera::Camera(glm::vec3 position) {
    this->cameraPos = position;

    this->cameraPos = glm::vec3(0.0f, 0.0f, 20.0f);
    this->cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    this->cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    this->firstMouse = true;
    this->yaw = -90.0f;
    this->pitch = 0.0f;
    this->lastX = 800.0f / 2.0f;
    this->lastY = 600.0 / 2.0f;
    this->fov = 45.0f;
}