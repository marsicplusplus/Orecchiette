#include "glad/glad.h"
#include "core/renderer.hpp"
#include "core/thread_pool.hpp"

#include "plog/Log.h"
#include "samplers/xorshift.hpp"
#include "cameras/perspective.hpp"
#include "managers/input_manager.hpp"

#define MAX_DEPTH 5

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	auto inputManager = InputManager::GetInstance();
	inputManager->setIsKeyDown(key, action != GLFW_RELEASE);
}

Renderer::Renderer(const Options &opt) : opts(opt), isInitialized(false), nFrames(0) {}

void Renderer::setScene(std::string &fp) {

}

void Renderer::setScene(std::shared_ptr<Scene> scene) {
	this->scene = scene;
}

void Renderer::drawGUI(){

}

void Renderer::start(){
	const int tHeight 			= this->opts.tileH;
	const int tWidth 			= this->opts.tileW;
	const int wWidth			= this->opts.width;
	const int wHeight			= this->opts.height;
	const int horizontalTiles	=  wWidth / tWidth;
	const int verticalTiles		= wHeight / tHeight;
	const int spp				= this->opts.spp;
	while(!glfwWindowShouldClose(this->window)){
		glfwPollEvents();
		auto inpManager = InputManager::GetInstance();
		if(inpManager->isKeyDown(GLFW_KEY_ESCAPE)){
			glfwSetWindowShouldClose(this->window, true);
		}
		this->lastUpdateTime = glfwGetTime();
		float frameTime = 0.0f;
		float lastTime = 0.0f;
		if (this->isBufferInvalid) {
			this->isBufferInvalid = false;
			this->nFrames = 0.0;
		}
		else {
			this->nFrames++;
		}
		std::vector<std::future<void>> futures;
		for (int tileRow = 0; tileRow < verticalTiles; ++tileRow) {
			for (int tileCol = 0; tileCol < horizontalTiles; ++tileCol) {
				futures.push_back(Threading::pool.queue([&, tileRow, tileCol, spp](std::shared_ptr<Sampler> sampler) {
					for (int row = 0; row < tHeight; ++row)
					{
						for (int col = 0; col < tWidth; ++col)
						{
							int x = col + tWidth * tileCol;
							int y = row + tHeight * tileRow;
							int idx = wWidth * y + x;
							Color c = BLACK;
							for (int i = 0; i < spp; ++i)
							{
								Ray ray;
								if (scene)
								{
									scene->getCamera()->getCameraRay(x, y, &ray, sampler);
									c += trace(ray);
								}
								else
								{
									auto t = 0.5f * (row + 1.0f);
									c += (1.0f - t) * glm::vec3(1.0, 1.0, 1.0) + t * glm::vec3(0.5, 0.7, 1.0);
								}
							}
							framebuffer.putPixel(idx, c / (float)spp, nFrames);
						}
					}
				}));
			}
		}
		for(auto& f : futures){
			f.get();
		}
		float now = glfwGetTime();
		frameTime = now - lastUpdateTime;
		lastTime = frameTime;
		lastUpdateTime = now;
		while(frameTime > 0.0) {
			float dt = std::min(frameTime, 1.0f/60.0f);
			frameTime -= dt;
			if(scene) this->isBufferInvalid = this->scene->update(dt);
		}	
		framebuffer.present();
		glfwSwapBuffers(this->window);
	}
}
#if 1
Color Renderer::trace(const Ray &ray, float lastSpecular, uint32_t depth) {
	Ray newRay;
	HitRecord hr;

	if (depth > MAX_DEPTH) {
		return BLACK;
	}

	if(scene->traverse(ray, EPS, INF, hr, sampler)){
#if 0
		glm::vec3 Ei = BLACK;
		for (int i = 0; i < scene->numberOfLights(); ++i){
			glm::vec3 pointOnLight, lightNormal;
			float lightArea;
			auto emitter = scene->sampleLight(sampler, i, pointOnLight, lightNormal, lightArea);
			glm::vec3 L = pointOnLight - hr.point;
			float dist = L.length();
			L /= dist;
			float cosO = glm::dot(-L, lightNormal);
			float cosI = glm::dot(L, hr.normal);
			if((cosO <= 0) || (cosI <= 0)) return BLACK;
			Ray r = Ray(hr.point + EPS * L, L);
			HitRecord hrDirect;
			if(!scene->traverse(r, EPS, INF, hrDirect, sampler)) return BLACK;
			auto material = scene->getMaterial(hr.materialIdx);
			glm::vec3 brdf = material->brdf(ray.direction, r.direction);
			float solidAngle = (cosO * emitter->area()) / (dist * dist);
			Ei += brdf * emitter->color * solidAngle * cosI;
		}
		return Ei * ((float)scene->numberOfLights());
#else
		auto material = scene->getMaterial(hr.materialIdx);
		float pdf;
		glm::vec3 brdf;
		if(material->reflect(ray, newRay, pdf, brdf, hr, sampler)) {
			glm::vec3 Ei = trace(newRay, lastSpecular, depth + 1) * glm::dot(hr.normal, newRay.direction) / pdf;
			return brdf * Ei;
		} else {
			return material->albedo;
		}
#endif
	} else {
		// No hit
		// sky
        // auto unitDir = ray.direction;
        // float t = 0.5f*(unitDir.y + 1.0f);
        // return ((1.0f-t) * glm::vec3(1.0f, 1.0f, 1.0f) + t * glm::vec3(0.5f, 0.7f, 1.0f)) * 0.3f;
		return BLACK;
	}
}
#else
Color Renderer::trace(const Ray &ray, float lastSpecular, uint32_t depth){
	Ray newRay;
	HitRecord hr;
#if 0
	if(scene->traverse(ray, EPS, INF, hr, sampler)){
		auto material = scene->getMaterial(hr.materialIdx);
		if(material->getType() == Mat::MaterialType::EMISSIVE){
			return lastSpecular * material->albedo;
		}

		glm::vec3 Ei(0.0f), brdf;
		float pdf = 1.0f;

		/* Indirect */
		material->reflect(ray, newRay, pdf, brdf, hr, sampler);
		Ei = (trace(newRay, 0.0f) * glm::dot(hr.normal, newRay.direction) / (float)pdf);

		/* Direct Sampling */
		glm::vec3 pointOnLight, normalOnLight;
		float A, distance;
		auto light = scene->sampleLights(sampler, pointOnLight, normalOnLight, A);
		glm::vec3 dir = glm::normalize(pointOnLight - hr.point);
		float lightP = 1.0f/(float)scene->numberOfLights();

		float dist = glm::distance(pointOnLight, hr.point);
		Ray occRay(hr.point + EPS*dir, dir);
		float NlL = glm::dot(normalOnLight, -dir);
		float NL = glm::dot(hr.normal, dir);
		glm::vec3 Ld(0.0f);
		bool occl = scene->isOccluded(occRay, light, dist);
		if(NL > 0 && NlL > 0 && !occl){
			float solidAngle = (NlL * A)/(dist*dist);
			Ld = (float) scene->numberOfLights() * light->color * light->intensity *  solidAngle * NL * brdf;
		}
		return PI * 2.0f * brdf * Ei + Ld;
	}
	return BLACK;
#else
	if(scene->traverse(ray, EPS, INF, hr, sampler)){
		auto material = scene->getMaterial(hr.materialIdx);
		if(material->getType() == Mat::MaterialType::EMISSIVE){
			return material->albedo * 1.0f;
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
#endif

bool Renderer::init() {
	glfwInit();
	PLOG_FATAL_IF(!glfwInit()) << "Cannot Initialize GLFW";

	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	if(opts.scaling < 1){
		GLFWmonitor *monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		this->window = glfwCreateWindow(mode->width, mode->height, opts.title.c_str(), NULL, NULL);
	} else {
		this->window = glfwCreateWindow((opts.width)*opts.scaling, opts.height*opts.scaling, opts.title.c_str(), NULL, NULL);
	}
	PLOG_FATAL_IF(this->window == nullptr) << "ERROR::Renderer::initSystems > could not create GLFW3 window;";
	glfwMakeContextCurrent(this->window);
	glfwSetKeyCallback(this->window, keyCallback);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	PLOG_FATAL_IF(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) << "Failed to initialize GLAD";
	sampler = std::make_shared<XorShift>(time(NULL));
	framebuffer.init(this->opts.width, this->opts.height);
	isInitialized = true;
	nFrames = 0;
	Threading::pool.init(8);
	return isInitialized;
}

Renderer::~Renderer(){}