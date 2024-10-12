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


struct SharedVec
{
public:
	void push_back(Chunk value)
	{
		// never use naked lock/unlock on mutex it is not exception safe
		std::scoped_lock<std::mutex> lock{ m };
		v.push_back(value);
	}

	void clear()
	{

		std::scoped_lock<std::mutex> lock{ m };
		v.clear();
	}

	std::vector<Chunk> get_copy()
	{

		std::scoped_lock<std::mutex> lock{ m };
		return std::vector<Chunk>{v};
	}

	void erase(int index)
	{

		std::scoped_lock<std::mutex> lock{ m };
		v.erase(v.begin() + index);
	
	}

	std::vector<Chunk>::iterator begin() {
		
		std::scoped_lock<std::mutex> lock{ m };
		return v.begin();
	}

	glm::vec2 getChunkPos(int i) {

		std::scoped_lock<std::mutex> lock{ m };
		return v.at(i).getChunkPos();
	}

	int size() {

		std::scoped_lock<std::mutex> lock{ m };
		return v.size();
	}
	void drawChunk(int i, glm::mat4 model, glm::mat4 projection, glm::mat4 view, glm::vec3 cameraPos) {
		std::scoped_lock<std::mutex> lock{ m };
		v.at(i).drawChunk(model, projection, view, cameraPos);
	}


private:
	// keep the lock with the object that you want to be threadsafe
	std::mutex m;
	std::vector<Chunk> v;

};


class ChunkLoader {


private:
	

	SharedVec chunks;
	glm::vec3 pPos;
	glm::vec2 playerCurrentChunk;

	glm::vec2 calculateCurrentChunkCoords();
	void checkChunkRange();
	

public:
	ChunkLoader();
	void manageChunks(glm::mat4 model, glm::mat4 projection, glm::mat4 view, glm::vec3 cameraPos, GLFWwindow* mainWindow);
	void deleteChunkBuffers();
	void threadBufferCreation(glm::vec2 cZone, GLFWwindow* mainWindow);
	void joinAllThreads();
};




#endif