#include "samplers/xorshift.hpp"

XorShift::XorShift(uint32_t seed) {
	state = seed;
}

float XorShift::getSample(){
	return xorShift() * 2.3283064365387e-10f; 
}

float XorShift::xorShift() {
	state ^= state << 13;
	state ^= state >> 17;
	state ^= state << 5;
	return state;
}
