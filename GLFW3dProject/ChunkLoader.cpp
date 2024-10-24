#include "ChunkLoader.h"
#include <thread>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <mutex>
#include <functional>
#include "ThreadPool.h"
#include "LayeredNoise.h"
#include <queue>

#define GLM_SWIZZLE


std::vector<std::thread> threads;
glm::vec3 pPos;
glm::vec2 playerCurrentChunk;
std::mutex vectorMutex;

glm::vec3 oldCoords = {0, 0, -10000};



std::vector<std::shared_ptr<Chunk>> chunks;

int frameCounter = 1000;
int frameCounter2 = 0;

int chunk_size = 32;




ChunkLoader::ChunkLoader() {
	
}

/*
Thread function for generating a new chunk.
glm::vec2 cZone - the x,z coords of the new chunk

TODO: remove mainWindow parameter (deprecated)
*/




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

void ChunkLoader::manageChunks(glm::mat4 model, glm::mat4 projection, glm::mat4 view, glm::vec3 cameraPos, ThreadPool *t, Feature *f) 
{
	
	
	glm::vec3 cCoords = calculateCurrentChunkCoords();

	pPos = cameraPos;

	frameCounter++;
	frameCounter2++;

		


		frameCounter = 0;
		
		


		bool cNew = false;


		int cloop = 0;
		int cx = 10;
		int cz = 10;
		int cy = 1;

		int cr1 = 0;
		int cr2 = 0;
	
		while (cloop < cx * cz * 7) {

			for (int x = -cr1; x < cr1 + 1; x++)
			{
				for (int z = -cr1; z < cr1 + 1; z++)
				{
					for (int y = 5; y >= 2; y--)
					{

						cloop++;
						if (!cNew) {
							glm::vec3 cZone = cCoords + glm::vec3((x * chunk_size), (y * chunk_size), (z * chunk_size));
							int chunksChecked = 0;
							bool built = false;

							for (int i = 0; i < chunks.size(); i++)
							{
								if (cZone == chunks.at(i)->getChunkPos()) {
									built = true;
								}
							}

							if (!built) {

								
								for (size_t x = 0; x < 32; x++)
								{
									for (size_t z = 0; z < 32; z++)
									{
										for (size_t y = 0; y < 32; y++) {
											srand(x * y + z);
											if (cZone.y + y < 120 && std::rand() % 100 == 2 && cZone.y + y == LayeredNoise::getHeightNoise(cZone.x + x, cZone.z + z) + 101) {
												f->createFeature(cZone.x + x, cZone.y + y, cZone.z + z, Feature::OAK_TREE);
											}
										}
									}
								}
								

								chunks.push_back(std::make_shared<Chunk>());
								chunks.at(chunks.size() - 1)->start(cZone.x, cZone.y, cZone.z, t, f);


							}

							chunksChecked++;
						}
					}
				}
			}

			cr1++;
			if (cr2 <= cy) {
				cr2++;
			}

		}

		



	




		
	for (size_t i = 0; i < chunks.size(); i++)
	{

		chunks.at(i)->drawChunk(model, projection, view, cameraPos);

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
glm::vec3 ChunkLoader::calculateCurrentChunkCoords() {

	int currentZ = (int)pPos.z;
	int currentX = (int)pPos.x;
	int currentY = (int)pPos.y;


	while (currentZ % chunk_size != 0) {
		if (currentZ > 0) {
			currentZ -= 1;
			
		}
		else {
			currentZ += 1;
			
		}

	}

	while (currentX % chunk_size != 0) {
		if (currentX > 0) {
			currentX -= 1;

		}
		else {
			currentX += 1;

		}

	}

	while (currentY % chunk_size != 0) {
		if (currentY > 0) {
			currentY -= 1;

		}
		else {
			currentY += 1;

		}

	}




	return glm::vec3(currentX - 16, 0, currentZ - 16);
}


/*
deletes all chunk buffers
*/
void ChunkLoader::deleteChunkBuffers() {
	for (int i = 0; i < chunks.size(); i++) {
		chunks.at(i)->deleteBuffers();
	}
}