#ifndef CHUNK_H
#define CHUNK_H


#include <vector>
#include <glm/glm.hpp>
#include "shader_s.h"
#include "TextureManager.h"
#include "ThreadPool.h"
#include "Feature.h"

class Chunk : public std::enable_shared_from_this<Chunk>
{
private:
	struct  b_look {

		TextureManager::blocks c_top;
		TextureManager::blocks c_bottom;
		TextureManager::blocks c_left;
		TextureManager::blocks c_right;
		TextureManager::blocks c_front;
		TextureManager::blocks c_back;

	};

	std::vector<b_look> block_table;

	bool chunkBuild;
	bool firstBake;
	char* hashedBlocked;
	size_t bSize = 32768;

	int vCount;
	int cCounter;
	


	unsigned int VBO, VAO;
	
	unsigned int texture;

	void pushToVertices(std::vector<float> eArray, std::vector<float> f, std::vector<float>* c);
	void populateBlockTable();
	bool loadTexture();

public:
	std::vector<float> chunk_verts;
	char* blocks;
	bool baked;
	bool bGen;
	Shader s1;
	int posX;
	int posZ;
	int posY;
	int v_size;
	float* vertices;
	

	Chunk();
	bool bakeChunk();
	bool isBaked();

	bool updateChunk();
	bool drawChunk(glm::mat4 model, glm::mat4 projection, glm::mat4 view, glm::vec3 cameraPos);
	glm::vec3 getChunkPos();
	void firstChunkGen(Feature *f);
	bool deleteBuffers();
	void genChunkMesh();
	void setChunkPos(int x, int y, int z);
	void setShaderMat4(char* attribute, glm::mat4 matrix);
	void generateBufferData();
	void deleteChunkData();
	void start(int x, int y, int z, ThreadPool *t, Feature *f);

	float d_below(glm::vec3 pos);

	float d_blockFront(glm::vec3 pos);
	float d_blockBehind(glm::vec3 pos);
	float d_blockLeft(glm::vec3 pos);
	float d_blockRight(glm::vec3 pos);

};


#endif