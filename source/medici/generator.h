#ifndef MEDICI_GENERATOR_H
#define MEDICI_GENERATOR_H

#include <medici/patience.h>
#include <mixer/mixer.h>
#include <type_traits>

namespace medici {

	struct Generator {
		struct TrueFunctor {
			template<class... Args>
			constexpr bool operator()(Args...) const {return true;}
		};

		template<std::size_t N, class TestBeforeFunctor = TrueFunctor, class TestAfterFunctor = TrueFunctor>
		static void Generate(std::array<Card, N>&, Patience::PatienceInfo& info, Mixer<Card, N>&, const TestBeforeFunctor& = TestBeforeFunctor(), const TestAfterFunctor& = TestAfterFunctor());
	};

}

#include "generator.hpp"

#endif /* MEDICI_GENERATOR_H */

