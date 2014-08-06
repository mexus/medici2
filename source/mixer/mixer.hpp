#ifndef MIXER_HPP
#define MIXER_HPP

#include "mixer.h"


template<class T, int N>
Mixer<T, N>::Mixer(uint_fast32_t seed) : randomEngine(seed), uniformDistribution(1, N - 1) {
}

template<class T, int N>
void Mixer<T, N>::Mix(std::array<T, N>& array){
	std::size_t i = uniformDistribution(randomEngine);
	std::swap(array[0], array[i]);
}

#endif /* MIXER_HPP */
