#include "glog/logging.h"
#include "core/renderer.hpp"

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
	Renderer renderer(opts);
	renderer.init();
	renderer.start();

	return 0;
}
