#ifndef __FRAMEBUFFER_HPP__
#define __FRAMEBUFFER_HPP__

#include "glm/vec3.hpp"

#include <cstdint>

class Framebuffer {
	public:
		Framebuffer() {};
		~Framebuffer();
		void init(uint64_t w, uint64_t h);
		void putPixel(uint64_t idx, const glm::vec3 &color);
		void present();

	private:
		uint64_t width;
		uint64_t height;
		unsigned int VAO, VBO, EBO;
		unsigned int shader;
		unsigned int texture;
		uint32_t *buffer;
};

#endif // __FRAMEBUFFER_HPP__
