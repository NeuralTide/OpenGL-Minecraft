#ifndef CHUNK_H
#define CHUNK_H


#include <vector>
#include <glm/glm.hpp>
#include "shader_s.h"
class Chunk
{
private:

	int posX;
	int posZ;
	bool chunkBuild;
	bool firstBake;
	char* hashedBlocked;
	size_t bSize = 65536;
	char* blocks = (char*)malloc(bSize * sizeof(char));;
	int vCount;
	int cCounter;
	bool baked;
	bool bGen;
	int v_size;	
	float* vertices;

	std::vector<float> chunk_verts;

	unsigned int VBO, VAO;
	Shader s1;
	unsigned int texture;

	int getLayeredNoise(int x, int z);
	void pushToVertices(float eArray[], std::vector<float>);
	void populateBlockTable();
	bool loadTexture();

public:
	Chunk();
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
};


#endif