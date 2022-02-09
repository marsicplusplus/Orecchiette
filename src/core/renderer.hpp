#ifndef __RENDERER_HPP__
#define __RENDERER_HPP__

#include "GLFW/glfw3.h"
#include "core/framebuffer.hpp"
#include "core/options.hpp"
#include "samplers/sampler.hpp"

#include <string>
#include <cstring>
#include <memory>

class Renderer{
	public:
		Renderer(const Options &opts);
		~Renderer();
		
		bool init();
		void start();

		void setScene(std::string &fp);

	private:
		void drawGUI();

		GLFWwindow *window;
		Framebuffer framebuffer;
		Options opts;
		std::shared_ptr<Sampler> sampler;
		bool isInitialized;
		uint64_t nFrames;
};

#endif //__RENDERER_HPP__
