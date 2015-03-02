#ifndef MEDICI_GENERATOR_H
#define MEDICI_GENERATOR_H

#include <medici/patience.h>
#include <mixer/mixer.h>
#include <type_traits>

namespace medici {

	namespace generator {

		template<std::size_t N>
		struct BeforeFunctor{
			virtual bool operator()(const std::array<Card, N>&) const {
				return true;
			}
		};

		template<std::size_t N>
		struct AfterFunctor{
			virtual bool operator()(const std::array<Card, N>&, const Patience::PatienceInfo&) const {
				return true;
			}
		};

		template<std::size_t N>
		void Generate(std::array<Card, N>&, Patience::PatienceInfo& info, Mixer<Card, N>&, const BeforeFunctor<N>& = BeforeFunctor<N>(), const AfterFunctor<N>& = AfterFunctor<N>());

	}

}

#include "generator.hpp"

#endif /* MEDICI_GENERATOR_H */

