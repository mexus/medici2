#ifndef MEDICI_GENERATOR_HPP
#define MEDICI_GENERATOR_HPP

#include "generator.h"

namespace medici{

	template<std::size_t N, class TestFunctor>
	void Generator::Generate(std::array<Card, N>& deck, Patience::PatienceInfo& info, Mixer<Card, N>& mixer, const TestFunctor& functor){
		do {
			mixer.Mix(deck);
			if (functor(deck, info) && Patience::Converge(deck, info))
				break;
		} while (true);
	}

}

#endif /* MEDICI_GENERATOR_HPP */

