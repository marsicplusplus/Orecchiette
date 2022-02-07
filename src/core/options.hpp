#ifndef __OPTIONS_HPP__
#define __OPTIONS_HPP__

#include <string>

class Options {
	public:
		Options();
		Options(std::string fp);
		
		std::string title;
		int width, height;
		int tileW, tileH;
		int spp;
};

#endif // __OPTIONS_HPP__
