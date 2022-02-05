#ifndef __RENDERER_HPP__
#define __RENDERER_HPP__

#include "core/framebuffer.hpp"

#include <string>

class Renderer{
	public:
		Renderer(std::string &title);
		
		bool init();
		void setScene(std::string &fp);
	private:
		void drawGUI();

		Framebuffer framebuffer;
		bool isInitialized;
};

#endif //__RENDERER_HPP__
