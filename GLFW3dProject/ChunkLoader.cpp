#include "ChunkLoader.h"
#include <thread>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <mutex>


#define GLM_SWIZZLE

std::vector<Chunk> chunks;
std::vector<std::thread> threads;
glm::vec3 pPos;
glm::vec2 playerCurrentChunk;
std::mutex vectorMutex;


/*
Thread function for generating a new chunk.
glm::vec2 cZone - the x,z coords of the new chunk

TODO: remove mainWindow parameter (deprecated)
*/

void ChunkLoader::threadBufferCreation(glm::vec2 cZone, GLFWwindow* mainWindow) 
{
	std::lock_guard<std::mutex> lock(vectorMutex);
	chunks.push_back(Chunk());
	chunks.at(chunks.size() - 1).setChunkPos(cZone.x, cZone.y);
	chunks.at(chunks.size() - 1).firstChunkGen();
	chunks.at(chunks.size() - 1).genChunkMesh();
}


/*
Main chunk function.
Runs through active chunks and despawns chunks which are too far from the player.
Checks to see if new chunks should be instantiated.
Finally draws any loaded chunks.

glm::mat4 model - current model mat4
glm::mat4 projection - current projection model mat4
glm::mat4 view - current view mat4
glm::vec3 cameraPos - current camera position

TODO:  GLFWwindow* mainWindow - deprecated, delete soon

*/
void ChunkLoader::manageChunks(glm::mat4 model, glm::mat4 projection, glm::mat4 view, glm::vec3 cameraPos, GLFWwindow* mainWindow) 
{
	glm::vec2 cCoords = calculateCurrentChunkCoords();
	pPos = cameraPos;
	for (int i = 0; i < chunks.size(); i++)
	{
		std::lock_guard<std::mutex> lock(vectorMutex);
		glm::vec2 cPos = chunks.at(i).getChunkPos();

		int distance = sqrt((pPos.x - cPos.x) * (pPos.x - cPos.x) + (pPos.z - cPos.y) * (pPos.z - cPos.y));
		
		if (distance > 190) 
		{
			chunks.erase(chunks.begin() + i);
		}
	}

	bool cNew = false;
	if (chunks.size() < 18) 
	{
		for (int x = -1; x < 2; x++)
		{
			for (int z = -1; z < 2; z++)
			{
				if (!cNew) {
					glm::vec2 cZone = cCoords + glm::vec2(x * 64, z * 64);
					int chunksChecked = 0;
					bool built = false;

					for (int i = 0; i < chunks.size(); i++)
					{
						if (cZone == chunks.at(i).getChunkPos()) {
							built = true;
						}
					}
					if (!built) {
						threads.push_back(std::thread(&ChunkLoader::threadBufferCreation, this, cZone, mainWindow));
						threads.at(threads.size() - 1).detach();
						cNew = true;

					}

					chunksChecked++;
				}
			}
		}
	}

	std::lock_guard<std::mutex> lock(vectorMutex);
	for (int i = 0; i < chunks.size(); i++) {
		chunks.at(i).drawChunk(model, projection, view, cameraPos);
	}
				
}


//remove
void ChunkLoader::checkChunkRange() { }

/*
finds and returns the actual chunk coord based off the players current position.
*/
glm::vec2 ChunkLoader::calculateCurrentChunkCoords() {

	int currentZ = (int)pPos.z;
	int currentX = (int)pPos.x;


	while (currentZ % 64 != 0) {
		if (currentZ > 0) {
			currentZ -= 1;
			
		}
		else {
			currentZ += 1;
			
		}

	}

	while (currentX % 64 != 0) {
		if (currentX > 0) {
			currentX -= 1;

		}
		else {
			currentX += 1;

		}

	}

	return glm::vec2(currentX, currentZ);
}


/*
deletes all chunk buffers
*/
void ChunkLoader::deleteChunkBuffers() {
	for (int i = 0; i < chunks.size(); i++) {
		chunks.at(i).deleteBuffers();
	}
}