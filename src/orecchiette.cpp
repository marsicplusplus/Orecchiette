#include "glog/logging.h"
#include "core/renderer.hpp"

int main(int argv, char* argc[]) {
	google::InitGoogleLogging(argc[0]);
	FLAGS_logtostderr = 1;

	Options options;
	options.width = 640;
	options.height = 384;
	options.title = "Default";

	Renderer renderer(options);
	renderer.init();
	renderer.start();

	return 0;
}
