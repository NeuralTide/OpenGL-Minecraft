#include "Feature.h"
#include <vector>



std::vector<std::vector<int>> f_vec;
enum Feature::feature_type { OAK_TREE = 0 };


void Feature::createFeature(int x, int y, int z, feature_type ft) {
	switch (ft)
	{
	case Feature::OAK_TREE:
		createTreeFeature(x, y, z);
	default:
		break;
	}

}
void Feature::createTreeFeature(int x, int y, int z) {
	std::vector<int> l1 = {	0, 0, 0, 0, 0,
							0, 0, 0, 0				};
}


Feature::Feature(int x, int y, int z, feature_type ft, int chunkDimX, int chunkDimY, int chunkDimZ) {
	createFeature(x, y, z, ft);
}