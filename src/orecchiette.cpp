#include "glog/logging.h"
#include "core/renderer.hpp"

#include "primitives/sphere.hpp"
#include "primitives/triangle.hpp"
#include "cameras/perspective.hpp"
#include "materials/emissive.hpp"
#include "materials/diffuse.hpp"

int main(int argv, char* args[]) {
	google::InitGoogleLogging(args[0]);
	FLAGS_logtostderr = 1;

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

	scene->addMaterial(std::make_shared<Mat::Diffuse>(RED));
	scene->addMaterial(std::make_shared<Mat::Diffuse>(glm::vec3(0.7, 0.5, 0.7)));
	scene->addMaterial(std::make_shared<Mat::Emissive>(WHITE));

	Transform t1;
	t1.translate(-0.6f, -1.5f, -2.0f);
	scene->addPrimitive(std::make_shared<Sphere>(t1, 0.5, 0));
	t1.translate(2.3f, 0.4, 0.0f);
	scene->addPrimitive(std::make_shared<Sphere>(t1, 0.8, 0));
	//scene->addPrimitive(std::make_shared<Sphere>(t2, 100.0, 1));

	// Light
	Transform t3;
	t3.translate(0,0,0);
	//scene->addPrimitive(std::make_shared<Sphere>(t3, 1.0, 2));
	t3.translate(3, 0, -3);
	//scene->addPrimitive(std::make_shared<Sphere>(t3, 0.5, 2));

	unsigned int planeIdxs[] = { 0, 1, 2, 0, 2, 3 };
	glm::vec3 p[] = { 
		glm::vec3(-10.5, -2, -10.5),
		glm::vec3(-10.5, -2, 10.5),
		glm::vec3(10.5, -2, 10.5),
		glm::vec3(10.5, -2, -10.5),
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
		nullptr,
		1);
	for (int i = 0; i < 2; ++i) {
		scene->addPrimitive(std::make_shared<Triangle>(trimesh, i, 1));
	}

	glm::vec3 p1[] = {
	glm::vec3(-2.5, 4, -2.5),
	glm::vec3(-2.5, 4, 2.5),
	glm::vec3(2.5, 4, 2.5),
	glm::vec3(2.5, 4, -2.5),
	};
	auto trimeshlight = std::make_shared<TriangleMesh>(t2,
		"PlaneLight", 2, 4,
		planeIdxs,
		p1,
		n,
		nullptr,
		1);
	for (int i = 0; i < 2; ++i) {
		scene->addPrimitive(std::make_shared<Triangle>(trimeshlight, i, 2));
	}

	std::shared_ptr<Camera::Camera> cam = std::make_unique<Camera::Perspective>(
				glm::vec3{0.0f, 0.0f, -7.0f},
				glm::vec3{0.0, 0.0, -1.0f},
				glm::vec2{opts.width, opts.height},
				90.0f
			);
	scene->setCamera(cam);
	
	Renderer renderer(opts);
	renderer.setScene(scene);
	renderer.init();
	renderer.start();
	return 0;
}
