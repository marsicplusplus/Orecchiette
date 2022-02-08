#ifndef __TEXTURE_HPP__
#define __TEXTURE_HPP__

#include "core/defs.hpp"
#include "glm/vec3.hpp"

#include <string>

enum class TextureType {
	SOLID_COLOR,
	IMAGE_TEXTURE,
	NIL,
};

class Texture {
	public:
		Texture(const std::string &_name) : name(_name) {}
		virtual ~Texture() = 0;

		virtual Color color(float u, float v, const glm::vec3 &p) const = 0;
		virtual TextureType getType() const = 0;

		inline const std::string& getName() const {
			return name;
		}


	private:
		std::string name; /* It was only needed when parsing the scene to associate materials to texture. Maybe we can avoid storing it once the scene is parsed; */
};

#endif
