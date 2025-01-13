#ifndef LOADER_HPP
#define LOADER_HPP

#include <string>
#include <vector>

namespace GLEngine {
	class Loader {
	public:
        static char* loadShader(const std::string& filePath);
		static bool loadObject(const std::string& filePath, std::vector<float>& vertices, std::vector<unsigned int>& indices);
	};
}
#endif
