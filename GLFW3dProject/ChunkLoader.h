#ifndef CHUNKLOADER_H
#define CHUNKLOADER_H

#include <vector>
#include <list>

#include "Chunk.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class ChunkLoader {

private:
	std::vector<Chunk> chunks;
	glm::vec3 pPos;
	glm::vec2 playerCurrentChunk;

	glm::vec2 calculateCurrentChunkCoords();
	void checkChunkRange();
	

public:
	void manageChunks(glm::mat4 model, glm::mat4 projection, glm::mat4 view, glm::vec3 cameraPos, GLFWwindow* mainWindow);
	void deleteChunkBuffers();
	void threadBufferCreation(glm::vec2 cZone, GLFWwindow* mainWindow);
};




#endif