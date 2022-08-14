#include "glad/glad.h"
#include "core/renderer.hpp"
#include "glog/logging.h"
#include "samplers/xorshift.hpp"
#include "cameras/perspective.hpp"
#include <iostream>

Renderer::Renderer(const Options &opt) : opts(opt){}

void Renderer::setScene(std::string &fp) {

}

void Renderer::setScene(std::shared_ptr<Scene> scene) {
	this->scene = scene;
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

	while(!glfwWindowShouldClose(this->window)){
		glfwPollEvents();
		this->nFrames++;
			for(int tileRow = 0; tileRow < verticalTiles; ++tileRow){
				for(int tileCol = 0; tileCol < horizontalTiles; ++tileCol){
					for (int row = 0; row < tHeight; ++row) {
						for (int col = 0; col < tWidth; ++col) {
							int x = col + tWidth * tileCol;
							int y = row + tHeight * tileRow;
							int idx = wWidth * y + x;
							Ray ray;
							Color color;
							if(scene){
								scene->getCamera()->getCameraRay(x, y, &ray, sampler);
								color = trace(ray);
							} else {
								auto t = 0.5f*(row + 1.0f);
								color = (1.0f-t)*glm::vec3(1.0, 1.0, 1.0) + t*glm::vec3(0.5, 0.7, 1.0);
							}
							framebuffer.putPixel(idx, color, nFrames);
						}
					}
				}
			}
			framebuffer.present();
			glfwSwapBuffers(this->window);
	}
}

Color Renderer::trace(const Ray &ray){
	Ray newRay;
	HitRecord hr;
	if(scene->traverse(ray, EPS, INF, hr, sampler)){
		auto material = scene->getMaterial(hr.materialIdx);
		if(material->getType() == Mat::MaterialType::EMISSIVE){
			return WHITE * 10.0f;
		} else if(material->getType() == Mat::MaterialType::DIFFUSE){
			material->reflect(ray, newRay, hr, sampler);
			auto Ei = trace(newRay) * glm::dot(hr.normal, newRay.direction);
			auto BRDF = material->albedo / PI;
			return PI * 2.0f * BRDF * Ei;
		}
	}
	return BLACK;
}

bool Renderer::init() {
	CHECK(glfwInit()) << "Cannot Initialize GLFW";

	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	if(opts.scaling < 1){
		GLFWmonitor *monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		CHECK(this->window = glfwCreateWindow(mode->width, mode->height, opts.title.c_str(), NULL, NULL)) << "ERROR::Renderer::initSystems > could not create GLFW3 window;";
	} else {
		CHECK(this->window = glfwCreateWindow((opts.width)*opts.scaling, opts.height*opts.scaling, opts.title.c_str(), NULL, NULL)) << "ERROR::Renderer::initSystems > could not create GLFW3 window";
	}
	glfwMakeContextCurrent(this->window);
	CHECK(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) << "Failed to initialize GLAD";
	sampler = std::make_shared<XorShift>(time(NULL));
	framebuffer.init(this->opts.width, this->opts.height);
	isInitialized = true;
	nFrames = 0;
	return isInitialized;
}

Renderer::~Renderer(){}
