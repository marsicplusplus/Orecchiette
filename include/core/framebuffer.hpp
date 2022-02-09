#ifndef __FRAMEBUFFER_HPP__
#define __FRAMEBUFFER_HPP__

#include "core/defs.hpp"
#include "glm/vec3.hpp"

#include <cstdint>

class Framebuffer {
	public:
		Framebuffer() {};
		~Framebuffer();
		/**
		 * Initialize the framebuffer by creating the texture and the uint32_t array that will store the pixel data.
		 *
		 * @param w The width of the final image;
		 * @param h The height of the final image;
		 */
		void init(uint64_t w, uint64_t h);
		/**
		 * Put a pixel in the Framebuffer.
		 * @param idx The index at which the color should be stored. Should be equal to width * y + x;
		 * @param color The color that must be put on screen.
		 * @param nFrames the number of frames rendered so far, used for average the accumulator; 
		 */
		void putPixel(uint64_t idx, const glm::vec3 &color, uint64_t nFrames);

		/**
		 * Pass the textured quad to the rendering pipeline;
		 */
		void present();

	private:
		uint64_t width;
		uint64_t height;
		unsigned int VAO, VBO, EBO;
		unsigned int shader;
		unsigned int texture;
		uint32_t *buffer;
		Color *accumulator;
};

#endif // __FRAMEBUFFER_HPP__
