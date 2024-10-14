#include "ChunkLoader.h"
#include <thread>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <mutex>
#include <functional>


#define GLM_SWIZZLE


std::vector<std::thread> threads;
glm::vec3 pPos;
glm::vec2 playerCurrentChunk;
std::mutex vectorMutex;

int frameCounter = 0;


struct SharedVec chunks;

ChunkLoader::ChunkLoader() {
	
}

/*
Thread function for generating a new chunk.
glm::vec2 cZone - the x,z coords of the new chunk

TODO: remove mainWindow parameter (deprecated)
*/



void ChunkLoader::threadBufferCreation(glm::vec2 cZone, GLFWwindow* mainWindow) 
{

	int cIndex = 0;
	Chunk nc = Chunk();
	cIndex = chunks.size();
	nc.setChunkPos(cZone.x, cZone.y);
	nc.firstChunkGen();
	nc.genChunkMesh();
	chunks.push_back(Chunk(nc));
	
	
	
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

	frameCounter++;
	
	if (frameCounter > 25) {
		frameCounter = 0;
		for (int i = 0; i < chunks.size(); i++)
		{

			glm::vec2 cPos = chunks.getChunkPos(i);

			int distance = sqrt((pPos.x - cPos.x + 32) * (pPos.x - cPos.x + 32) + (pPos.z - cPos.y + 32) * (pPos.z - cPos.y + 32));

			if (distance > 288)
			{
				chunks.erase(i);
			}
		}

		bool cNew = false;


	
		if (chunks.size() < 81)
		{
			int cloop = 0;
			int cx = 9;
			int cr = 1;
			while (cloop < cx * cx) {

				for (int x = -cr; x < cr + 1; x++)
				{
					for (int z = -cr; z < cr + 1; z++)
					{
						cloop++;
						if (!cNew) {
							glm::vec2 cZone = cCoords + glm::vec2(x * 64, z * 64);
							int chunksChecked = 0;
							bool built = false;

							for (int i = 0; i < chunks.size(); i++)
							{
								if (cZone == chunks.getChunkPos(i)) {
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

				cr++;
			}
		}


	}


	
	for (size_t i = 0; i < chunks.size(); i++)
	{
		chunks.drawChunk(i, model, projection, view, cameraPos);

	}
		



				
}


void ChunkLoader::joinAllThreads() {
	for (size_t i = 0; i < threads.size(); i++)
	{
		if (threads.at(i).joinable()) {
			threads.at(i).join();
		}

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
	for (int i = 0; i < chunks.get_copy().size(); i++) {
		chunks.get_copy().at(i).deleteBuffers();
	}
}