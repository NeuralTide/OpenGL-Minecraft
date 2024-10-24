#ifndef FEATURE_H
#define FEATURE_H

#include <vector>
#include <map>
#include "glm/glm.hpp"

class Feature {



private:

	struct Vec3Comparator {
		bool operator()(const glm::vec3& lhs, const glm::vec3& rhs) const {
			if (lhs.x != rhs.x) return lhs.x < rhs.x;
			if (lhs.y != rhs.y) return lhs.y < rhs.y;
			return lhs.z < rhs.z;
		}
	};

	std::vector<char> oak_tree_l1 = { 'A', 'A', 'A', 'A', 'A',
								'A', 'A', 'A', 'A', 'A',
								'A', 'A', 'L', 'A', 'A',
								'A', 'A', 'A', 'A', 'A',
								'A', 'A', 'A', 'A', 'A', };
	std::vector<char> oak_tree_l2 = { 'A', 'A', 'A', 'A', 'A',
								'A', 'A', 'A', 'A', 'A',
								'A', 'A', 'L', 'A', 'A',
								'A', 'A', 'A', 'A', 'A',
								'A', 'A', 'A', 'A', 'A', };
	std::vector<char> oak_tree_l3 = { 'F', 'F', 'F', 'F', 'F',
								'F', 'F', 'F', 'F', 'F',
								'F', 'F', 'L', 'F', 'F',
								'F', 'F', 'F', 'F', 'F',
								'F', 'F', 'F', 'F', 'F', };
	std::vector<char> oak_tree_l4 = { 'F', 'F', 'F', 'F', 'F',
								'F', 'F', 'F', 'F', 'F',
								'F', 'F', 'L', 'F', 'F',
								'F', 'F', 'F', 'F', 'F',
								'F', 'F', 'F', 'F', 'F', };
	std::vector<char> oak_tree_l5 = { 'A', 'A', 'A', 'A', 'A',
								'A', 'F', 'F', 'F', 'A',
								'A', 'F', 'L', 'F', 'A',
								'A', 'F', 'F', 'F', 'A',
								'A', 'A', 'A', 'A', 'A', };
	std::vector<char> oak_tree_l6 = { 'A', 'A', 'A', 'A', 'A',
								'A', 'A', 'F', 'A', 'A',
								'A', 'F', 'F', 'F', 'A',
								'A', 'A', 'F', 'A', 'A',
								'A', 'A', 'A', 'A', 'A', };
	std::vector<std::vector<int>> f_vec;

	std::map<glm::vec3, char, Vec3Comparator> fmap;

	
	


	
	void createOakTreeFeature(int x, int y, int z);

public:
	enum feature_type { OAK_TREE = 0 };
	Feature();
	void createFeature(int x, int y, int z, feature_type ft);
	char getFeatureBlock(int x, int y, int z);
};



#endif