#include "TextureManager.h"
#include "glm/glm.hpp"
#include <vector>





enum blocks { DIRT, GRASS_SIDE, GRASS_TOP, SAND, STONE, BEDROCK, LOG, LOG_TOP, LEAF, SNOW};
enum faces { TOP, BOTTOM, LEFT, RIGHT, FRONT, BACK };
int size;
const float OFFSET = 0.2f;




/*
	Untransformed texturecoordinates for a 6x6 cube.
*/

//left face
float left_coords[] = { 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f };
//right face
float right_coords[] = { 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f };
//front
float front_coords[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f };
//back
float back_coords[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f };
//top
float top_coords[] = { 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
//bottom
float bottom_coords[] = { 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };

/*
	Constructs a new TextureManager object 
	int size - the dimensions (size*size) of the the textureatlas
*/
TextureManager::TextureManager(int size) {
	this->size = size;
}


/*
	Constructs a std::vector<float> of texture coordiantes for the relevant block face.
	blocks b - the block enum of the relevant block face
	faces f - the face enum of the relevant block face

*/
std::vector<float> TextureManager::getFaceCoords(blocks b, faces f) {

	float r[12] = {};
	std::vector<float> toReturn;

	switch (f)
	{
	case TOP:
		for (int i = 0; i < 12; i++)
		{
			r[i] = top_coords[i];
		}

		break;
	case BOTTOM:
		for (int i = 0; i < 12; i++)
		{
			r[i] = bottom_coords[i];
		}
		break;
	case LEFT:
		for (int i = 0; i < 12; i++)
		{
			r[i] = left_coords[i];
		}
		break;
	case RIGHT:
		for (int i = 0; i < 12; i++)
		{
			r[i] = right_coords[i];
		}
		break;
	case FRONT:
		for (int i = 0; i < 12; i++)
		{
			r[i] = front_coords[i];
		}
		break;
	case BACK:
		for (int i = 0; i < 12; i++)
		{
			r[i] = back_coords[i];
		}
		break;
	default:
		break;
	}



	switch (b)
	{
	case DIRT:
		for (int i = 0; i < 12; i++)
		{

			if (i % 2 == 0)
				r[i] = (r[i] / size) + (OFFSET * 5);
			if (i % 2 != 0)
				r[i] = (r[i] / size) - OFFSET;

		}
		break;
	case GRASS_SIDE:



		for (size_t i = 0; i < 12; i++)
		{
			//y
			if (i % 2 == 0)
				r[i] = (r[i] / (size + .1)) + (OFFSET)+0.00125f;

			//x
			if (i % 2 != 0)
				r[i] = (r[i] / (size + .1)) - (OFFSET);

		}
		break;
	case GRASS_TOP:
		for (size_t i = 0; i < 12; i++)
		{
			//y
			if (i % 2 == 0)
				r[i] = (r[i] / (size + .1)) + (OFFSET * 2) + 0.00125f;

			//x
			if (i % 2 != 0)
				r[i] = (r[i] / (size + .1)) - (OFFSET);
		}
		break;
	case SAND:
		for (size_t i = 0; i < 12; i++)
		{
			if (i % 2 == 0)
				r[i] = (r[i] / size) + (OFFSET * 3);
			if (i % 2 != 0)
				r[i] = (r[i] / size);
		}
		break;
	case STONE:
		for (size_t i = 0; i < 12; i++)
		{
			//y
			if (i % 2 == 0)
				r[i] = (r[i] / (size + .1)) + (OFFSET * 4) + 0.00125f;

			//x
			if (i % 2 != 0)
				r[i] = (r[i] / (size + .1)) - (OFFSET);
		}
		break;
	case BEDROCK:
		for (size_t i = 0; i < 12; i++)
		{
			//y
			if (i % 2 == 0)
				r[i] = (r[i] / (size + .1)) + 0.00125f;

			//x
			if (i % 2 != 0)
				r[i] = (r[i] / (size + .1)) - (OFFSET * 2);
		}
		break;
	case LOG:
		for (size_t i = 0; i < 12; i++)
		{
			//y
			if (i % 2 == 0)
				r[i] = (r[i] / (size + .1)) + (OFFSET)+0.00125f;

			//x
			if (i % 2 != 0)
				r[i] = (r[i] / (size + .1)) - (OFFSET * 2);
		}
		break;
	case LOG_TOP:
		for (size_t i = 0; i < 12; i++)
		{
			//y
			if (i % 2 == 0)
				r[i] = (r[i] / (size + .1)) + (OFFSET * 2) + 0.00125f;

			//x
			if (i % 2 != 0)
				r[i] = (r[i] / (size + .1)) - (OFFSET * 2);
		}
		break;
	case LEAF:
		for (size_t i = 0; i < 12; i++)
		{
			//y
			if (i % 2 == 0)
				r[i] = (r[i] / (size + .1)) + (OFFSET * 3) + 0.00125f;

			//x
			if (i % 2 != 0)
				r[i] = (r[i] / (size + .1)) - (OFFSET * 2);
		}
		break;


	case SNOW:
		for (size_t i = 0; i < 12; i++)
		{
			//y
			if (i % 2 == 0)
				r[i] = (r[i] / (size + .1)) + (OFFSET * 4) + 0.00125f;

			//x
			if (i % 2 != 0)
				r[i] = (r[i] / (size + .1)) - (OFFSET * 2);
		}
		break;
	}

	for (int i = 0; i < 12; i++)
	{
		toReturn.push_back(r[i]);
	}

	return toReturn;
}