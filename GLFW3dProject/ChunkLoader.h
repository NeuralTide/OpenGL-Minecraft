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
#include <mutex>
#include "ThreadPool.h"
#include "Feature.h"


class ChunkLoader {


private:
	



	glm::vec2 playerCurrentChunk;
	std::vector<std::shared_ptr<Chunk>> chunks;
	glm::vec3 calculateCurrentChunkCoords();
	void checkChunkRange();
	

public:
	glm::vec3 pPos;
	ChunkLoader();
	void manageChunks(glm::mat4 model, glm::mat4 projection, glm::mat4 view, glm::vec3 cameraPos, ThreadPool *t, Feature *f);
	void deleteChunkBuffers();
	void threadBufferCreation(glm::vec3 cZone, GLFWwindow* mainWindow);
	void joinAllThreads();
};




#endif