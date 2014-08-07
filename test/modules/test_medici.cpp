#include "test_medici.h"

logxx::Log TestMedici::cLog("TestMedici");

TestMedici::TestMedici() : TestFW("medici"){
}

bool TestMedici::Tests(){
	return false;
}

bool TestMedici::Test(const ArrayType& deck, bool etalonConverges, const Medici::PatienceInfo& etalonInfo){
	S_LOG("Test");
	Medici::PatienceInfo resultInfo;
	bool resultConverges = Medici::Converge<DeckType::N()>(deck, resultInfo);

	if (resultConverges != etalonConverges){
		auto &s = log(logxx::error) << "Deck should ";
		if (!etalonConverges)
			s << "not ";
		s << "converge, but it does ";
		if (!resultConverges)
			s << "not";
		s << logxx::endl;
		return false;
	} else
		return Compare(resultInfo, etalonInfo);
}

namespace {
	std::ostream& operator<<(std::ostream& s, const Card& card){
		return s << "s " << card.suit << ", r " << card.rank;
	}
}

bool TestMedici::Compare(const Medici::PatienceInfo& etalon, const Medici::PatienceInfo& result){
	return TestFW::Compare(etalon.convolutions, result.convolutions) &&
		TestFW::Compare(etalon.mobiles, result.mobiles) &&
		TestFW::Compare(etalon.stationars, result.stationars);
}

