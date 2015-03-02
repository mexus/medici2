#ifndef MEDICI_GENERATOR_HPP
#define MEDICI_GENERATOR_HPP

#include "generator.h"

namespace medici{

	namespace generator {

		template<std::size_t N>
		void Generate(std::array<Card, N>& deck, Patience::PatienceInfo& info, Mixer<Card, N>& mixer, const BeforeFunctor<N>& beforeFunctor, const AfterFunctor<N>& afterFunctor){
			do {
				mixer.Mix(deck);
			} while (!(beforeFunctor(deck) && Patience::Converge(deck, info) && afterFunctor(deck, info)));
		}

	}

}

#endif /* MEDICI_GENERATOR_HPP */

