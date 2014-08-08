#ifndef MEDICI_GENERATOR_H
#define MEDICI_GENERATOR_H

#include <medici/patience.h>
#include <mixer/mixer.h>
#include <type_traits>

namespace medici {

	struct BinaryTrueFunctor {
		template<class A, class B>
		bool operator()(const A&, const B&) const {return true;}
	};

	struct Generator {
		template<std::size_t N, class TestFunctor = BinaryTrueFunctor>
		static void Generate(std::array<Card, N>&, Patience::PatienceInfo& info , Mixer<Card, N>&);
	};

}

#include "generator.hpp"

#endif /* MEDICI_GENERATOR_H */

