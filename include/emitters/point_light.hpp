#ifndef __POINT_LIGHT_HPP__
#define __POINT_LIGHT_HPP__

#include "emitters/emitter.hpp"

class PointLight : public Emitter {
		public:
			PointLight(const Transform &o2w, Color c) : 
				Emitter(o2w, c) {
					pos = o2w.getMatrix() * glm::vec4(0.0, 0.0, 0.0, 1.0);
				};
			glm::vec3 li(std::shared_ptr<Sampler> &sampler, HitRecord &hr, Ray &vRay, glm::vec3 &wi, float &pdf, float &dist) const override;

		private:
			glm::vec3 pos;
};

#endif
