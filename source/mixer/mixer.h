#ifndef MIXER_H
#define MIXER_H

#include <random>
#include <array>

template<class T, std::size_t N>
class Mixer {
public:
	Mixer(uint_fast32_t seed = 0);
	Mixer(const Mixer&) = default;
	Mixer(const Mixer&, uint_fast32_t seed);
	void Mix(std::array<T, N>&);
	void Shuffle(std::array<T, N>&);
private:
	std::minstd_rand randomEngine;
	std::uniform_int_distribution<std::size_t> uniformDistribution;
};

#include "mixer.hpp" 

#endif /* MIXER_H */
