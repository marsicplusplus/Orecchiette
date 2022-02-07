#include "glad/glad.h"
#include "glog/logging.h"
#include "renderer.hpp"
#include "samplers/xorshift.hpp"
#include "cameras/perspective.hpp"

Renderer::Renderer(const Options &opt) : opts(opt){}

void Renderer::setScene(std::string &fp) {

}

void Renderer::drawGUI(){

}

void Renderer::start(){
	const int tHeight 	= this->opts.tileH;
	const int tWidth 	= this->opts.tileW;
	const int wWidth	= this->opts.width;
	const int wHeight	= this->opts.height;
	const int horizontalTiles =  wWidth / tWidth;
	const int verticalTiles = wHeight / tHeight;

	std::unique_ptr<Camera> cam = std::make_unique<Perspective>(
				glm::vec3{0.0f, 0.0f, 3.0f},
				glm::vec3{0.0, 0.0, -1.0f},
				glm::vec2{this->opts.width, this->opts.height},
				90.0f
			);
	while(!glfwWindowShouldClose(this->window)){
		glfwPollEvents();
		for(int tileRow = 0; tileRow < verticalTiles; ++tileRow){
			for(int tileCol = 0; tileCol < horizontalTiles; ++tileCol){
				for (int row = 0; row < tHeight; ++row) {
					for (int col = 0; col < tWidth; ++col) {
						int x = col + tWidth * tileCol;
						int y = row + tHeight * tileRow;
						int idx = wWidth * y + x;
						Ray ray;
						cam->getCameraRay(x, y, &ray, sampler);
						auto t = 0.5f*(ray.direction.y + 1.0f);
						auto color = (1.0f-t)*glm::vec3(1.0, 1.0, 1.0) + t*glm::vec3(0.5, 0.7, 1.0);
						framebuffer.putPixel(idx, color);
					}
				}
			}
		}
		framebuffer.present();
		glfwSwapBuffers(this->window);
	}
}

bool Renderer::init() {
	CHECK(glfwInit()) << "Cannot Initialize GLFW";

	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	CHECK(this->window = glfwCreateWindow(this->opts.width, this->opts.height, ("Orecchiette | " + this->opts.title).c_str(), NULL, NULL)) << "Could not create GLFW3 window\n";
	glfwMakeContextCurrent(this->window);
	CHECK(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) << "Failed to initialize GLAD";
	sampler = std::make_shared<XorShift>(time(NULL));
	framebuffer.init(this->opts.width, this->opts.height);
	isInitialized = true;
	return isInitialized;
}

Renderer::~Renderer(){
	glfwDestroyWindow(this->window);
}
