#ifndef FEATURE_H
#define FEATURE_H

#include <vector>


class Feature {

private:

	std::vector<std::vector<int>> f_vec;
	enum feature_type;


	void createFeature(int x, int y, int z, feature_type ft);
	void createTreeFeature(int x, int y, int z);

public:
	
	Feature(int x, int y, int z, feature_type ft, int chunkDimX, int chunkDimY, int chunkDimZ);
};



#endif