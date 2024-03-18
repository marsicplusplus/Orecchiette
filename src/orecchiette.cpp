#include "core/renderer.hpp"
#include "plog/Log.h"
#include "plog/Initializers/RollingFileInitializer.h"
#include "shapes/sphere.hpp"
#include "shapes/triangle.hpp"
#include "cameras/perspective.hpp"
#include "materials/emissive.hpp"
#include "materials/diffuse.hpp"

int main(int argv, char* args[]) {
	plog::init(plog::debug, "log.csv", 100000, 5);

	std::string configPath = "";
	for(int i = 1; i < argv; i++){
		if(strncmp("config=", args[i], strlen("config=")) == 0){
			configPath = (&args[i][strlen("config=")]);
		}
	}

	Options opts;
	if(!configPath.empty()){
		opts.parseFromFile(configPath);
	}
	std::shared_ptr<Scene> scene = std::make_shared<Scene>();

	PLOG_INFO << "Orecchiette; Config file: " << configPath;

	scene->addMaterial(std::make_shared<Mat::Diffuse>(RED));						// 0
	scene->addMaterial(std::make_shared<Mat::Diffuse>(glm::vec3(2.7, 2.5, 2.7)));	// 1
	scene->addMaterial(std::make_shared<Mat::Emissive>(WHITE * 5.0f));						// 2
	scene->addMaterial(std::make_shared<Mat::Diffuse>(GREEN));						// 3

	// Spheres
	Transform t1;
	t1.translate(-1.5f, 1.0f, -3.0f);
	std::shared_ptr sphereShape1 = std::make_shared<Sphere>(t1, 1.0);
	std::shared_ptr<Primitive> sphere1 = std::make_shared<Primitive>(sphereShape1, 0);
	scene->addPrimitive(sphere1);
	Transform transf;
	transf.translate(glm::vec3(1.4f, 0.8f, -1.4));
	scene->addPrimitive(
		std::make_shared<Primitive>(std::make_shared<Sphere>(transf, 0.8), 
		3)
	);
	// Lights
	Transform lightTransform;
	lightTransform.translate(glm::fvec3(-1, 2.4, -3));
	scene->addPrimitive(std::make_shared<Primitive>(
		std::make_shared<Sphere>(lightTransform, 0.4), 
		2)
	);
	Transform lightTransform1;
	lightTransform1.translate(glm::fvec3(0.5, 2.0, -2));
	scene->addPrimitive(std::make_shared<Primitive>(
		std::make_shared<Sphere>(lightTransform1, 0.6), 
		2)
	);

	// Plane
	unsigned int planeIdxs[] = { 0, 1, 2, 0, 2, 3 };
	glm::vec3 p[] = { 
		glm::vec3(-10.5, 0, -10.5),
		glm::vec3(-10.5, 0, 10.5),
		glm::vec3(10.5, 0, 10.5),
		glm::vec3(10.5, 0, -10.5),
	};
	glm::vec3 n[] = {
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
	};
	Transform t2;
	auto trimesh = std::make_shared<TriangleMesh>(t2, 
		"Plane", 2, 4,
		planeIdxs,
		p,
		n,
		nullptr);
	for (int i = 0; i < 2; ++i) {
		scene->addPrimitive(std::make_shared<Primitive>(std::make_shared<Triangle>(trimesh, i), 1));
	}

	// Camera
	std::shared_ptr<Camera::Camera> cam = std::make_unique<Camera::Perspective>(
				glm::vec3{-0.3f, 0.4f, 1.4f}, 			// Origin
				glm::vec3{0.0, 0.0, -1.0f},				// LookAt
				glm::vec3{0.0, 1.0, 0.0f},				// Up
				glm::vec2{opts.width, opts.height},		// Screenbounds
				90.0f									// Fov
			);
	scene->setCamera(cam);
	
	Renderer renderer(opts);
	renderer.setScene(scene);
	renderer.init();
	renderer.start();
	return 0;
}