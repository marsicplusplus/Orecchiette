#include "glog/logging.h"

int main(int argv, char* argc[]) {
	google::InitGoogleLogging(argc[0]);
	FLAGS_logtostderr = 1;


	return 0;
}
