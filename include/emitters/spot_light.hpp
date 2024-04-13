#ifndef __SPOT_LIGHT_HPP__
#define __SPOT_LIGHT_HPP__

#include "emitters/emitter.hpp"

class SpotLight : public Emitter {
		public:
			SpotLight(const Transform &o2w, Color c, float cosWidth, float cosFallof) : 
				Emitter(o2w, c),
                m_cosTotalWidth(std::cos(glm::radians(cosWidth))), 
                m_cosFalloffStart(std::cos(glm::radians(cosFallof))) {
					m_pos = o2w.transformPoint(glm::vec4(0.0, 0.0, 0.0, 1.0));
				};
			glm::vec3 li(std::shared_ptr<Sampler> &sampler, HitRecord &hr, Ray &vRay, glm::vec3 &wi, float &pdf, float &dist) const override;

		private:
			glm::vec3 m_pos;
            float m_cosTotalWidth, m_cosFalloffStart;

			float falloffLight(glm::vec3 &w) const;
};

#endif

