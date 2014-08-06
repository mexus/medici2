#ifndef MIXER_H
#define MIXER_H

#include <random>
#include <array>

template<class T, int N>
class Mixer {
public:
	Mixer(uint_fast32_t seed = 0);
	void Mix(std::array<T, N>&);
private:
	std::ranlux24_base randomEngine;
	std::uniform_int_distribution<std::size_t> uniformDistribution;
};

#include "mixer.hpp" 

#endif /* MIXER_H */
