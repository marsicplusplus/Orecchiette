#ifndef __OPTIONS_HPP__
#define __OPTIONS_HPP__

#include <string>

class Options {
	public:
		Options();
		
		void parseFromFile(std::string fp);

		std::string title;
		int width, height;
		int tileW, tileH;
		int scaling;
		int nThreads;
		int spp;
};

#endif // __OPTIONS_HPP__
