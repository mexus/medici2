#ifndef MIXER_HPP
#define MIXER_HPP

#include "mixer.h"
#include <algorithm>


template<class T, std::size_t N>
Mixer<T, N>::Mixer(uint_fast32_t seed) : randomEngine(seed), uniformDistribution(1, N - 1) {
}

template<class T, std::size_t N>
Mixer<T, N>::Mixer(const Mixer& other, uint_fast32_t seed) : randomEngine(seed), uniformDistribution(other.uniformDistribution) {
}

template<class T, std::size_t N>
void Mixer<T, N>::Mix(std::array<T, N>& array){
	std::size_t i = uniformDistribution(randomEngine);
	std::swap(array[0], array[i]);
}

template<class T, std::size_t N>
void Mixer<T, N>::Shuffle(std::array<T, N>& array){
	std::shuffle(array.begin(), array.end(), randomEngine);
}



#endif /* MIXER_HPP */
