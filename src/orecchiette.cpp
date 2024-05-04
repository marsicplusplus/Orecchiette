#include "core/renderer.hpp"
#include "shapes/sphere.hpp"
#include "shapes/xzrect.hpp"
#include "shapes/triangle.hpp"
#include "cameras/perspective.hpp"
#include "materials/dielectric.hpp"
#include "materials/emissive.hpp"
#include "materials/diffuse.hpp"
#include "materials/mirror.hpp"
#include "emitters/point_light.hpp"
#include "emitters/spot_light.hpp"
#include "emitters/directional.hpp"


#include <stdio.h>
#include <thread>
#include "samplers/xorshift.hpp"

int main(int argv, char* args[]) {
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

	scene->addMaterial(std::make_shared<Mat::Mirror>(WHITE));						// 0
	scene->addMaterial(std::make_shared<Mat::Diffuse>(glm::vec3(0.7, 0.5, 0.7)));	// 1
	scene->addMaterial(std::make_shared<Mat::Emissive>(glm::vec3(1.2, 1.2, 2.8) * 1.0f));				// 2
	scene->addMaterial(std::make_shared<Mat::Diffuse>(GREEN));						// 3
	scene->addMaterial(std::make_shared<Mat::Diffuse>(glm::vec3(0.8, 0.8, 0.8)));	// 4
	scene->addMaterial(std::make_shared<Mat::Emissive>(WHITE * 1.0f));				// 5
	scene->addMaterial(std::make_shared<Mat::Dielectric>(glm::vec3(1.0, 1.0, 1.0), 1.49f));				// 6 GLASS

	// Spheres
	Transform t1;
	t1.translate(-1.5f, 1.0f, -3.0f);
	std::shared_ptr sphereShape1 = std::make_shared<Sphere>(t1, 1.0);
	std::shared_ptr<Primitive> sphere1 = std::make_shared<Primitive>(sphereShape1, 6);
	scene->addPrimitive(sphere1);
	Transform transf;
	transf.translate(glm::vec3(1.4f, 0.8f, -1.4));
	scene->addPrimitive(
		std::make_shared<Primitive>(std::make_shared<Sphere>(transf, 0.8), 
		3)
	);
	Transform sphereTransform;
	sphereTransform.translate(glm::fvec3(-2.4, 1.0, -4.5));	
	scene->addPrimitive(std::make_shared<Primitive>(
		std::make_shared<Sphere>(sphereTransform, 1.0), 
		1)
	);

	Transform mirrorTran;
	mirrorTran.scale(3.0f);	
	mirrorTran.translate(glm::fvec3(4.5, 1.5, -4.0));	
	mirrorTran.rotate(glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));	
	mirrorTran.rotate(glm::radians(35.0f), glm::vec3(1.0, 0.0, 0.0));	
	scene->addPrimitive(std::make_shared<Primitive>(
		std::make_shared<XZRect>(mirrorTran), 
		0)
	);

	// Lights
	Transform lightTransform2;
	lightTransform2.scale(5.0f);	
	lightTransform2.translate(glm::fvec3(-4.5, 1.0, -3.0));	
	lightTransform2.rotate(glm::radians(-90.0f), glm::vec3(0.0, 0.0, 1.0));	
	scene->addPrimitive(std::make_shared<Primitive>(
		std::make_shared<XZRect>(lightTransform2), 
		5)
	);
	
	Transform lightTransform;
	lightTransform.translate(glm::fvec3(2.4, 4.0, -4.5));	
	scene->addPrimitive(std::make_shared<Primitive>(
		std::make_shared<Sphere>(lightTransform, 0.5), 
		5)
	);

	// Floor
	Transform t2;
	t2.scale(20);
	scene->addPrimitive(std::make_shared<Primitive>(
		std::make_shared<XZRect>(t2),
		4
	));

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