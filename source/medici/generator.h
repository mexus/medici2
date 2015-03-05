#ifndef MEDICI_GENERATOR_H
#define MEDICI_GENERATOR_H

#include <medici/patience.h>
#include <mixer/mixer.h>

#include <type_traits>
#include <atomic>

namespace medici {

	namespace generator {

		struct True {
			template<class...Args>
			bool operator()(Args...) const {
				return true;
			}
		};

		template<std::size_t N, class BeforeFunctor = True, class AfterFunctor = True>
		void Generate(std::array<Card, N>& deck, Patience::PatienceInfo& info, Mixer<Card, N>& mixer,
				const std::atomic_bool& interrupt,
				const BeforeFunctor& before = BeforeFunctor(),
				const AfterFunctor& after = AfterFunctor())
		{
			do {
				mixer.Mix(deck);
			} while (!interrupt && !(before(deck) && Patience::Converge(deck, info) && after(deck, info)));
		}

	}

}

#endif /* MEDICI_GENERATOR_H */

