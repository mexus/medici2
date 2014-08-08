#ifndef MEDICI_GENERATOR_HPP
#define MEDICI_GENERATOR_HPP

#include "generator.h"

template<std::size_t N, class TestFunctor>
void MediciGenerator::Generate(std::array<Card, N>& deck, Medici::PatienceInfo& info, Mixer<Card, N>& mixer){
	do {
		mixer.Mix(deck);
		if (Medici::Converge(deck, info) && TestFunctor()(deck, info))
			break;
	} while (true);
}

#endif /* MEDICI_GENERATOR_HPP */

