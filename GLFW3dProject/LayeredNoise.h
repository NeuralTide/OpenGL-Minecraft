#pragma once
#ifndef LAYEREDNOISE_H
#define LAYEREDNOISE_H

#include "SimplexNoise.h";

class LayeredNoise {
private:

public:
	static float getCaveNoise(int x, int y, int z);
	static float getCaveNoise_2(int x, int y, int z);

	static int getHeightNoise(int x, int z);

};



#endif