#include "core/options.hpp"
#include <thread>
#include <fstream>

Options::Options() {
	width 		= 768;
	height 		= 432;
	tileH 		= 16;
	tileW 		= 16;
	title 		= "Orecchiette";
	scaling		= 1;
	nThreads 	= 1;
	spp 		= 1;
}

void Options::parseFromFile(std::string fp) {
	std::ifstream config(fp);
	std::string line;
	while(std::getline(config, line)){
		if(line[0] == '#') continue;
		std::string key = line.substr(0, line.find('='));
		line.erase(0, line.find('=') + 1);
		if(key == "SAMPLES") 		spp 	= std::stoi(line);
		if(key == "TILE_WIDTH") 	tileW 	= std::stoi(line);
		if(key == "TILE_HEIGHT") 	tileH 	= std::stoi(line);
		if(key == "W_HEIGHT") 		height 	= std::stoi(line);
		if(key == "W_WIDTH")		width 	= std::stoi(line);
		if(key == "SCALING") 		scaling = std::stoi(line);
		if(key == "THREADS"){
			int nThreadsTmp = std::stoi(line);
			// std::thread::hardware_concurrency() can return 0 on failure 
			int hardwareConcurrency = std::thread::hardware_concurrency();
			if(hardwareConcurrency != 0){
				nThreadsTmp = (nThreadsTmp < 1) ? hardwareConcurrency : (std::min)(hardwareConcurrency, nThreadsTmp);
			}else{
				nThreadsTmp = 1;
			}
									nThreads = nThreadsTmp;
		}
	}
}
