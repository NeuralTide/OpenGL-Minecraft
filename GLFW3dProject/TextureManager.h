#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "glm/glm.hpp"
#include <vector>


class TextureManager {
private:
	int size;

public:

	enum blocks { DIRT, GRASS_SIDE, GRASS_TOP, SAND, STONE, BEDROCK, LOG, LOG_TOP, LEAF, SNOW };
	enum faces { TOP, BOTTOM, LEFT, RIGHT, FRONT, BACK };

	TextureManager(int size);
	std::vector<float> getFaceCoords(blocks b, faces f);

};



#endif