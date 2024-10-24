#include "Feature.h"
#include <vector>






void Feature::createFeature(int x, int y, int z, feature_type ft) {
	switch (ft)
	{
	case Feature::OAK_TREE:
		createOakTreeFeature(x, y, z);
		break;
	
	}

}
void Feature::createOakTreeFeature(int x, int y, int z) {
	

	int xc = -2;
	int zc = -2;
	int counter = 0;
	for (size_t i = 0; i < oak_tree_l1.size(); i++)
	{
		fmap.emplace(std::make_pair(glm::vec3(x + xc, y, z + zc), oak_tree_l1.at(i)));
		fmap.emplace(std::make_pair(glm::vec3(x + xc, y + 1, z + zc), oak_tree_l2.at(i)));
		fmap.emplace(std::make_pair(glm::vec3(x + xc, y + 2, z + zc), oak_tree_l3.at(i)));
		fmap.emplace(std::make_pair(glm::vec3(x + xc, y + 3, z + zc), oak_tree_l4.at(i)));
		fmap.emplace(std::make_pair(glm::vec3(x + xc, y + 4, z + zc), oak_tree_l5.at(i)));
		fmap.emplace(std::make_pair(glm::vec3(x + xc, y + 5, z + zc), oak_tree_l6.at(i)));
		counter++;
		xc++;

		if (counter >= 5) {
			zc++;
			xc = -2;
			counter = 0;
		}

		
		
		
	}
}

char Feature::getFeatureBlock(int x, int y, int z) {

	auto it = fmap.find(glm::vec3(x, y, z));

	if (it != fmap.end()) {
		return it->second;
	}
	else {
		return '!';
	}
	
}


Feature::Feature() {
	
}

