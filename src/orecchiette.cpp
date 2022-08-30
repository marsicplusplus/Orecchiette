#include "glog/logging.h"
#include "core/renderer.hpp"

#include "primitives/sphere.hpp"
#include "cameras/perspective.hpp"
#include "materials/emissive.hpp"

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
	Transform t1;
	scene->addPrimitive(std::make_shared<Sphere>(t1, 1.0));
	Transform t2;
	t2.translate(0.0f, -2.0, 0);
	scene->addPrimitive(std::make_shared<Sphere>(t2, 1.0));
	
	scene->addMaterial(std::make_shared<Mat::Emissive>(WHITE));
	Transform t3;
	t3.translate(-3,-1, 2);
	//scene->addPrimitive(std::make_shared<Sphere>(t3, 1.0, 1));

	Transform t4;
	t4.translate(4, 2, 1);
	scene->addPrimitive(std::make_shared<Sphere>(t4, 1.0, 1));

	std::shared_ptr<Camera::Camera> cam = std::make_unique<Camera::Perspective>(
				glm::vec3{0.0f, 0.0f, -5.0f},
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
