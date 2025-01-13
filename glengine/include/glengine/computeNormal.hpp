#ifndef COMPUTE_NORMAL_HPP
#define COMPUTE_NORMAL_HPP

#include <vector>

namespace GLEngine {
	class ComputeNormal {
	public:
        static std::vector<float> computeNormal(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
	};
}
#endif
