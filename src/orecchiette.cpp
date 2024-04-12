#include "core/renderer.hpp"
#include "plog/Log.h"
#include "plog/Initializers/RollingFileInitializer.h"
#include "shapes/sphere.hpp"
#include "shapes/xzrect.hpp"
#include "shapes/triangle.hpp"
#include "cameras/perspective.hpp"
#include "materials/emissive.hpp"
#include "materials/diffuse.hpp"
#include "emitters/point_light.hpp"
#include "emitters/directional.hpp"

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
	scene->addMaterial(std::make_shared<Mat::Diffuse>(glm::vec3(0.8, 0.8, 0.8)));						// 0
	scene->addMaterial(std::make_shared<Mat::Emissive>(BLUE * 10.0f));						// 2

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
	// Transform lightTransform;
	// lightTransform.translate(glm::fvec3(-1, 3.0, -3));
	// scene->addPrimitive(std::make_shared<Primitive>(
	// 	std::make_shared<Sphere>(lightTransform, 0.4), 
	// 	2)
	// );

	Transform lightTransform1;
	lightTransform1.translate(glm::fvec3(0.2, 3.2, -1.4));
	// scene->addPrimitive(std::make_shared<Primitive>(
	// 	std::make_shared<Sphere>(lightTransform1, 0.8), 
	// 	2)
	// );

	// scene->addLight(
	// 	std::make_shared<PointLight>(lightTransform1, WHITE * 10.0f)
	// );
	scene->addLight(
		std::make_shared<Directional>(glm::vec3(0.8, 0.0, -.2), WHITE * 2.0f)
	);

	Transform t2;
	t2.scale(10.5, 1, 15.5);
	t2.translate(0.0, 0.0, -5.0);
	// t2.rotate(-180.0, glm::vec3(1.0, 0.0, 0.0));
	scene->addPrimitive(std::make_shared<Primitive>(
		std::make_shared<XZRect>(t2),
		4
	));

	// Transform wallLeft;
	// wallLeft.translate(-3.5, 0.0, 0.0);
	// wallLeft.scale(15.0);
	// wallLeft.rotate(glm::radians(-90.0f), glm::vec3(0.0, 0.0, 1.0));
	// scene->addPrimitive(std::make_shared<Primitive>(
	// 	std::make_shared<XZRect>(wallLeft),
	// 	4
	// ));

	// Transform wallRight;
	// wallRight.translate(3.5, 0.0, 0.0);
	// wallRight.scale(15.0);
	// wallRight.rotate(glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	// scene->addPrimitive(std::make_shared<Primitive>(
	// 	std::make_shared<XZRect>(wallRight),
	// 	4
	// ));

	// Transform lightPlane;
	// lightPlane.translate(0.0, 1.0, -2.0);
	// lightPlane.scale(1.0, 1.0, 0.5);
	// lightPlane.rotate(glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
	// scene->addPrimitive(std::make_shared<Primitive>(
	// 	std::make_shared<XZRect>(lightPlane),
	// 	5
	// ));

	// Transform wallBack;
	// wallBack.translate(0.0, 0.0, -5.0);
	// wallBack.scale(15.0);
	// wallBack.rotate(glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
	// scene->addPrimitive(std::make_shared<Primitive>(
	// 	std::make_shared<XZRect>(wallBack),
	// 	4
	// ));

	// Camera
	std::shared_ptr<Camera::Camera> cam = std::make_unique<Camera::Perspective>(
				glm::vec3{-0.0f, 1.4f, 1.0f}, 			// Origin
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