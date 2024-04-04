#include "glad/glad.h"
#include "core/renderer.hpp"
#include "core/thread_pool.hpp"

#include "plog/Log.h"
#include "samplers/xorshift.hpp"
#include "cameras/perspective.hpp"
#include "managers/input_manager.hpp"

#define MAX_DEPTH 5

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	auto inputManager = InputManager::GetInstance();
	inputManager->setIsKeyDown(key, action != GLFW_RELEASE);
}

Renderer::Renderer(const Options &opt) : opts(opt), isInitialized(false), nFrames(0) {}

void Renderer::setScene(std::string &fp)
{
}

void Renderer::setScene(std::shared_ptr<Scene> scene)
{
	this->scene = scene;
}

void Renderer::drawGUI()
{
}

void Renderer::start()
{
	const int tHeight = this->opts.tileH;
	const int tWidth = this->opts.tileW;
	const int wWidth = this->opts.width;
	const int wHeight = this->opts.height;
	const int horizontalTiles = wWidth / tWidth;
	const int verticalTiles = wHeight / tHeight;
	const int spp = this->opts.spp;
	while (!glfwWindowShouldClose(this->window))
	{
		glfwPollEvents();
		auto inpManager = InputManager::GetInstance();
		if (inpManager->isKeyDown(GLFW_KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(this->window, true);
		}
		this->lastUpdateTime = glfwGetTime();
		float frameTime = 0.0f;
		float lastTime = 0.0f;
		if (this->isBufferInvalid)
		{
			this->isBufferInvalid = false;
			this->nFrames = 0.0;
		}
		else
		{
			this->nFrames++;
		}
		std::vector<std::future<void>> futures;
		for (int tileRow = 0; tileRow < verticalTiles; ++tileRow)
		{
			for (int tileCol = 0; tileCol < horizontalTiles; ++tileCol)
			{
				futures.push_back(Threading::pool.queue([&, tileRow, tileCol, spp](std::shared_ptr<Sampler> sampler)
														{
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
					} }));
			}
		}
		for (auto &f : futures)
		{
			f.get();
		}
		float now = glfwGetTime();
		frameTime = now - lastUpdateTime;
		lastTime = frameTime;
		lastUpdateTime = now;
		while (frameTime > 0.0)
		{
			float dt = std::min(frameTime, 1.0f / 60.0f);
			frameTime -= dt;
			if (scene)
				this->isBufferInvalid = this->scene->update(dt);
		}
		framebuffer.present();
		glfwSwapBuffers(this->window);
	}
}

#define INDIRECT_ONLY false
Color Renderer::trace(const Ray &ray, float lastSpecular, uint32_t depth)
{
	HitRecord hr;
	if (depth > MAX_DEPTH)
	{
		return BLACK;
	}

	if (scene->traverse(ray, EPS, INF, hr, sampler))
	{
		auto material = scene->getMaterial(hr.materialIdx);
		auto primitive = scene->getPrimitive(hr.geomIdx);
		glm::vec3 Ei = BLACK;
#if not INDIRECT_ONLY
		if (primitive->light != nullptr)
		{									// We hit a light
			return primitive->light->color; // light->Le();
		}
		glm::vec3 directLight(0.0);
		std::shared_ptr<Emitter> light;
		uint64_t lightIdx = 0;
		while (true)
		{
			float f = sampler->getSample();
			uint64_t i = std::max(0, std::min(scene->numberOfLights() - 1, (int)floor(f * scene->numberOfLights())));
			light = scene->getEmitter(i);
			if (primitive->light != light)
				break;
		}
		light = scene->getEmitter(lightIdx);
		float pdf;
		glm::vec3 wi;
		Ray visibilityRay;
		auto li = light->li(sampler, hr, visibilityRay, wi, pdf);
		if (scene->visibilityCheck(visibilityRay, EPS, INF, sampler))
		{
			directLight = li * (float)scene->numberOfLights();
		}
		float reflectionPdf;
		glm::vec3 brdf;
		Ray newRay;
		material->reflect(ray, newRay, reflectionPdf, brdf, hr, sampler);
		Ei = trace(newRay, lastSpecular, depth + 1) * glm::dot(hr.normal, newRay.direction) / reflectionPdf;
		return brdf * (Ei + directLight);
#else
		float reflectionPdf;
		glm::vec3 brdf;
		Ray newRay;
		material->reflect(ray, newRay, reflectionPdf, brdf, hr, sampler);
		Ei = trace(newRay, lastSpecular, depth + 1) * glm::dot(hr.normal, newRay.direction) / reflectionPdf;
		return brdf * (Ei);
#endif
	}
	else
	{
		// No hit
		return BLACK;
	}
}

bool Renderer::init()
{
	glfwInit();
	PLOG_FATAL_IF(!glfwInit()) << "Cannot Initialize GLFW";

	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	if (opts.scaling < 1)
	{
		GLFWmonitor *monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode *mode = glfwGetVideoMode(monitor);
		this->window = glfwCreateWindow(mode->width, mode->height, opts.title.c_str(), NULL, NULL);
	}
	else
	{
		this->window = glfwCreateWindow((opts.width) * opts.scaling, opts.height * opts.scaling, opts.title.c_str(), NULL, NULL);
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
	Threading::pool.init(std::thread::hardware_concurrency() - 1.0);
	return isInitialized;
}

Renderer::~Renderer() {}