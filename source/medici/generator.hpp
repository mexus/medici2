#ifndef MEDICI_GENERATOR_HPP
#define MEDICI_GENERATOR_HPP

#include "generator.h"

namespace medici{

	template<std::size_t N, class TestBeforeFunctor, class TestAfterFunctor>
	void Generator::Generate(std::array<Card, N>& deck, Patience::PatienceInfo& info, Mixer<Card, N>& mixer, const TestBeforeFunctor& beforeFunctor, const TestAfterFunctor& afterFunctor){
		do {
			mixer.Mix(deck);
		} while (!(beforeFunctor(deck) && Patience::Converge(deck, info) && afterFunctor(deck, info)));
	}

}

#endif /* MEDICI_GENERATOR_HPP */

