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

Color Renderer::trace(const Ray &ray, float lastSpecular){
	Ray newRay;
	HitRecord hr;
#if 0
	if(scene->traverse(ray, EPS, INF, hr, sampler)){
		auto material = scene->getMaterial(hr.materialIdx);
		if(material->getType() == Mat::MaterialType::EMISSIVE){
			return lastSpecular * material->albedo;
		}

		/* Indirect */
		glm::vec3 Ei(0.0f), brdf;
		float pdf = 1.0f;
		//if(material->getType() == Mat::MaterialType::DIFFUSE){
			material->reflect(ray, newRay, pdf, brdf, hr, sampler);
			Ei = (trace(newRay, 0.0f) * glm::dot(hr.normal, newRay.direction) / (float)pdf);
		//}

		/* Direct Sampling */
		glm::vec3 pointOnLight, normalOnLight;
		float A, distance;
		auto light = scene->sampleLights(sampler, pointOnLight, normalOnLight, A);
		glm::vec3 dir = glm::normalize(pointOnLight - hr.point);

		float dist = glm::distance(pointOnLight, hr.point);
		Ray occRay(hr.point + EPS*dir, dir);
		float NlL = glm::dot(normalOnLight, -dir);
		float NL = glm::dot(hr.normal, dir);
		glm::vec3 Ld(0.0f);
		bool occl = scene->isOccluded(occRay, light, dist);
		if(NL > 0 && NlL > 0 && !occl){
			float solidAngle = (NlL * A)/(dist*dist);
			Ld = light->color * light->intensity *  solidAngle * NL * brdf;
		}

		return PI * 2.0f * brdf * Ei + Ld;
	}
	return BLACK;
#else
	if(scene->traverse(ray, EPS, INF, hr, sampler)){
		auto material = scene->getMaterial(hr.materialIdx);
		if(material->getType() == Mat::MaterialType::EMISSIVE){
			return WHITE * 10.0f;
		} else if(material->getType() == Mat::MaterialType::DIFFUSE){
			glm::vec3 brdf;
			float pdf;
			material->reflect(ray, newRay, pdf, brdf, hr, sampler);
			auto Ei = trace(newRay) * glm::dot(hr.normal, newRay.direction)/pdf;
			return PI * 2.0f * brdf * Ei;
		}
	}
	return BLACK;
#endif
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
