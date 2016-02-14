#include "performance.h"

#include <easylogging++.h>

#include <cards/and-condition.h>
#include <cards/any-suit-condition.h>
#include <cards/apply-to-range-condition.h>
#include <cards/one-card-condition.h>
#include <cards/or-condition.h>

#include <helpers/sequence-mixer.h>
#include <helpers/time_measure.h>
#include <i-ching/i-ching.h>
#include <mixer/factory.h>

using namespace cards;
using namespace medici;
using namespace helpers;

std::vector<cards::Sequence> Performance::default_sequences_;

Performance::Performance() : mixers_factory_36_(N) {}

void Performance::Run(MixersFactory::MixerType mixer_type) {
    mixer_type_ = mixer_type;
    mixers_factory_36_.SetMixer(mixer_type);
    Mixing();
    ConditionGenerator();
    MediciGenerator();
    MediciWithConditions();
    MediciWithConditionsAndIChing();
    IChingBalancedPercent();
}

void Performance::Mixing() const {
    auto mixer = GetMixer36();
    auto deck = standard_36_deck::kDeck;
    static const std::size_t decksCount = 1E6;
    TimeMeasure timer;
    for (std::size_t i = 0; i != decksCount; ++i) {
        mixer->Mix(deck);
    }
    double elapsed = timer.SecondsElapsed();
    LOG(INFO) << decksCount << " decks generated in " << elapsed
              << "s: " << decksCount / elapsed << " decks per second";
}

// DeckSelectors Performance::DefaultSelectors() {
//     using namespace standard_36_deck;
//     DeckOneSelector targetCard({Selector(Hearts, Ten)}, 19, 24);
//     DeckAllSelector ownActions({SelectorAnySuit(Ace, false)}, 3, 7);
//     DeckOneSelector firstCard({SelectorAnySuit(Jack)}, 0, 0);
//     DeckOneSelector secondCard({SelectorAnySuit(Nine)}, 1, 1);
//     DeckOneSelector thirdCard({SelectorAnySuit(Ace), SelectorAnySuit(Ten)}, 2, 2);
//
//     DeckSelectors selectors;
//     selectors.AddDeckSelector(targetCard);
//     selectors.AddDeckSelector(ownActions);
//     selectors.AddDeckSelector(firstCard);
//     selectors.AddDeckSelector(secondCard);
//     selectors.AddDeckSelector(thirdCard);
//     return selectors;
// }

std::shared_ptr<Condition> Performance::DefaultPreCondition() {
    using namespace standard_36_deck;
    auto main_condition = std::make_shared<AndCondition>();

    auto target_card = std::make_shared<OneCardCondition>(0, Card{Hearts, Ten});
    auto target_card_range = std::make_shared<OrCondition>();
    target_card_range->AddChild<MoveCondition>(19, target_card);
    target_card_range->AddChild<MoveCondition>(20, target_card);
    target_card_range->AddChild<MoveCondition>(21, target_card);
    target_card_range->AddChild<MoveCondition>(22, target_card);
    main_condition->AddChild(target_card_range);

    main_condition->AddChild<AnySuitCondition>(0, Jack);  // First card
    main_condition->AddChild<AnySuitCondition>(1, Nine);  // Second card

    auto third_card = std::make_shared<OrCondition>();
    third_card->AddChild<AnySuitCondition>(2, Ace);
    third_card->AddChild<AnySuitCondition>(2, Ten);
    main_condition->AddChild(third_card);

    main_condition->AddChild<ApplyToRangeCondition>(
        3, 3, std::make_shared<AnySuitCondition>(0, Ace, true));  // Own actions

    return main_condition;
}

std::shared_ptr<Condition> Performance::DefaultPostCondition() {
    using namespace standard_36_deck;
    auto main_condition = std::make_shared<ApplyToRangeCondition>(
        3, 7, std::make_shared<AnySuitCondition>(0, Ace, true));  // Own actions
    return main_condition;
}

void Performance::ConditionGenerator() const {
    if (!default_sequences_.empty()) {
        return ;
    }
    using namespace standard_36_deck;
    auto conditions = DefaultPreCondition();
    auto deck = standard_36_deck::kDeck;
    cards::Storage storage(deck);

    TimeMeasure timer;
    auto variants = conditions->GetVariants({}, storage);
    double elapsed = timer.SecondsElapsed();
    LOG(INFO) << variants.size() << " variants generated in " << elapsed << "s";
    if (variants.empty()) {
        throw std::logic_error("No variants generated");
    }
    default_sequences_ = std::move(variants);
}

std::vector<std::vector<Card>> Performance::PregenerateConvergableDecks() const {
    auto mixer = GetMixer36();
    PatienceInfo info;
    std::vector<std::vector<Card>> pregeneratedDecks;
    static const std::size_t decksCount = 1E3;
    pregeneratedDecks.reserve(decksCount);
    auto deck = standard_36_deck::kDeck;
    TimeMeasure timer;
    for (std::size_t i = 0; i != decksCount; ++i) {
        do {
            mixer->Mix(deck);
        } while (!TryToConverge(deck, info));
        pregeneratedDecks.push_back(deck);
    }
    double elapsed = timer.SecondsElapsed();
    LOG(INFO) << decksCount << " convergable decks generated in " << elapsed
              << "s: " << decksCount / elapsed << " decks per second";
    return pregeneratedDecks;
}

void Performance::MediciGenerator() const {
    PatienceInfo info;

    auto pregeneratedDecks = PregenerateConvergableDecks();
    auto it = pregeneratedDecks.cbegin();
    auto end = pregeneratedDecks.cend();

    TimeMeasure timer;
    for (; it != end; ++it) {
        auto& deck = *it;
        if (!TryToConverge(deck, info))
            throw std::logic_error("Convergable deck doesn't converges!");
    }
    double elapsed = timer.SecondsElapsed();
    LOG(INFO) << pregeneratedDecks.size() << " convergable decks converged in " << elapsed
              << "s: " << pregeneratedDecks.size() / elapsed << " decks per second";
}

void Performance::MediciWithConditions() const {
    auto sequence = default_sequences_[0];
    Storage storage(standard_36_deck::kDeck);
    sequence.TakeCardsFromStorage(&storage);

    size_t free_spots = storage.GetAllAvailableCards().size();
    MixersFactory factory(free_spots);
    factory.SetMixer(mixer_type_);
    auto mixer = factory.CreateMixer<Card*, helpers::SwapDereference>(0);

    SequenceMixer seq_mixer(sequence, storage, mixer);

    auto post_conditions = DefaultPostCondition();

    const size_t totalDecks = 1000;
    TimeMeasure timer;
    auto& deck = seq_mixer.GetDeck();
    PatienceInfo info;
    for (size_t i = 0; i != totalDecks; ++i) {
        do {
            seq_mixer.Mix();
        } while (!(TryToConverge(deck, info) && post_conditions->CheckSequence(deck)));
    }
    double elapsed = timer.SecondsElapsed();
    LOG(INFO) << totalDecks << " appropriate decks generated in " << elapsed
              << "s: " << totalDecks / elapsed << " decks per second";
}

void Performance::MediciWithConditionsAndIChing() const {
    auto sequence = default_sequences_[0];
    Storage storage(standard_36_deck::kDeck);
    sequence.TakeCardsFromStorage(&storage);

    size_t free_spots = storage.GetAllAvailableCards().size();
    MixersFactory factory(free_spots);
    factory.SetMixer(mixer_type_);
    auto mixer = factory.CreateMixer<Card*, helpers::SwapDereference>(0);

    SequenceMixer seq_mixer(sequence, storage, mixer);

    auto post_conditions = DefaultPostCondition();

    i_ching::BalanceChecker balance_checker;
    const size_t totalDecks = 1000;
    size_t balanced = 0;
    TimeMeasure timer;
    auto& deck = seq_mixer.GetDeck();
    PatienceInfo info;
    for (size_t i = 0; i != totalDecks; ++i) {
        do {
            seq_mixer.Mix();
        } while (!(TryToConverge(deck, info) && post_conditions->CheckSequence(deck)));
        if (balance_checker.Check(info)) {
            ++balanced;
        }
    }
    double elapsed = timer.SecondsElapsed();
    LOG(INFO) << totalDecks << " appropriate decks generated in " << elapsed
              << "s: " << totalDecks / elapsed << " decks per second";
    LOG(INFO) << "  -- " << balanced << " balanced decks";
}

void Performance::IChingBalancedPercent() const {
    auto sequence = default_sequences_[0];
    Storage storage(standard_36_deck::kDeck);
    sequence.TakeCardsFromStorage(&storage);

    size_t free_spots = storage.GetAllAvailableCards().size();
    MixersFactory factory(free_spots);
    factory.SetMixer(mixer_type_);
    auto mixer = factory.CreateMixer<Card*, helpers::SwapDereference>(0);

    SequenceMixer seq_mixer(sequence, storage, mixer);

    auto post_conditions = DefaultPostCondition();

    i_ching::BalanceChecker balance_checker;
    const size_t totalDecks = 20000;
    size_t balanced = 0;
    TimeMeasure timer;
    auto& deck = seq_mixer.GetDeck();
    PatienceInfo info;
    for (size_t i = 0; i != totalDecks; ++i) {
        do {
            seq_mixer.Mix();
        } while (!TryToConverge(deck, info));
        // } while (!(TryToConverge(deck, info) && post_conditions->CheckSequence(deck)));
        if (balance_checker.Check(info)) {
            ++balanced;
        }
    }
    double elapsed = timer.SecondsElapsed();
    LOG(INFO) << totalDecks << " appropriate decks generated in " << elapsed
              << "s: " << totalDecks / elapsed << " decks per second";
    LOG(INFO) << "  -- " << balanced
              << " balanced decks: " << static_cast<double>(balanced) / totalDecks * 100.0
              << "%";
}

std::shared_ptr<MixerInterface<Card>> Performance::GetMixer36(
    std::uint_fast32_t seed) const {
    return mixers_factory_36_.CreateMixer<Card>(seed);
}

// std::shared_ptr<MixerInterface<Card*, helpers::SwapDereference>>
// Performance::GetMixerDeref(std::uint_fast32_t seed) const {
//     return mixers_factory_.CreateMixer<Card*, helpers::SwapDereference>(seed);
// }
