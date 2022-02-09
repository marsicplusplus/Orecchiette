#ifndef __XORSHIFT_HPP__
#define __XORSHIFT_HPP__

#include "samplers/sampler.hpp"
#include <cstdint>

class XorShift : public Sampler {
	public:
		XorShift(uint32_t seed = 0);
		float getSample();
	private:
		float xorShift();
		uint32_t state;
};

#endif
