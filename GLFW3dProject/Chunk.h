#ifndef CHUNK_H
#define CHUNK_H


#include <vector>
#include <glm/glm.hpp>
#include "shader_s.h"
class Chunk
{
private:


	bool chunkBuild;
	bool firstBake;
	char* hashedBlocked;
	size_t bSize = 65536;

	int vCount;
	int cCounter;
	



	unsigned int VBO, VAO;
	
	unsigned int texture;

	int getLayeredNoise(int x, int z);
	void pushToVertices(float eArray[], std::vector<float>);
	void populateBlockTable();
	bool loadTexture();

public:
	std::vector<float> chunk_verts;
	char* blocks = (char*)malloc(bSize * sizeof(char));
	bool baked;
	bool bGen;
	Shader s1;
	int posX;
	int posZ;
	int v_size;
	float* vertices;


	Chunk();
	Chunk(const Chunk& copy);
	bool bakeChunk();
	bool isBaked();

	bool updateChunk();
	bool drawChunk(glm::mat4 model, glm::mat4 projection, glm::mat4 view, glm::vec3 cameraPos);
	glm::vec2 getChunkPos();
	bool firstChunkGen();
	bool deleteBuffers();
	bool genChunkMesh();
	void setChunkPos(int x, int y);
	void setShaderMat4(char* attribute, glm::mat4 matrix);
	void generateBufferData();

	float d_below(glm::vec3 pos);

	float d_blockFront(glm::vec3 pos);
	float d_blockBehind(glm::vec3 pos);
	float d_blockLeft(glm::vec3 pos);
	float d_blockRight(glm::vec3 pos);

};


#endif