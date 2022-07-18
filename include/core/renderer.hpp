#ifndef __RENDERER_HPP__
#define __RENDERER_HPP__

#include "GLFW/glfw3.h"
#include "samplers/sampler.hpp"
#include "core/framebuffer.hpp"
#include "core/options.hpp"
#include "core/scene.hpp"

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
		void setScene(const std::shared_ptr<Scene> scene);

	private:
		void drawGUI();
		Color trace(const Ray &ray);

		GLFWwindow *window;
		Framebuffer framebuffer;
		Options opts;
		std::shared_ptr<Sampler> sampler;
		std::shared_ptr<Scene> scene;
		bool isInitialized;
		uint64_t nFrames;
};

#endif //__RENDERER_HPP__
