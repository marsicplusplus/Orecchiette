#ifndef __POINT_HPP__
#define __POINT_HPP__

#include "emitters/emitter.hpp"

class Point : public Emitter {
		public:
			Point(const Transform &o2w, Color c) : 
				Emitter(o2w, c) {};
		glm::vec3 randomPoint();
};

#endif
