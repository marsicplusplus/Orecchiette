#include "emitters/point.hpp"

glm::vec3 Point::randomPoint(){
	return o2w.getTranslation();
}
