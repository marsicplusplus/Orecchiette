#ifndef __SPOT_LIGHT_HPP__
#define __SPOT_LIGHT_HPP__

#include "emitters/emitter.hpp"

class SpotLight : public Emitter {
		public:
			SpotLight(const Transform &o2w, Color c, float cosWidth, float cosFallof) : 
				Emitter(o2w, c),
                cosTotalWidth(std::cos(glm::radians(cosWidth))), 
                cosFalloffStart(std::cos(glm::radians(cosFallof))) {
					pos = o2w.getMatrix() * glm::vec4(0.0, 0.0, 0.0, 1.0);
				};
			glm::vec3 li(std::shared_ptr<Sampler> &sampler, HitRecord &hr, Ray &vRay, glm::vec3 &wi, float &pdf) const override;

		private:
			glm::vec3 pos;
            float cosTotalWidth, cosFalloffStart;
};

#endif

