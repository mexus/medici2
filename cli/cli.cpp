#include "cli.h"
#include <helpers/time_measure.h>
#include <cards/deck-selector.h>
#include <i-ching/i-ching.h>
#include <mixer/factory.h>
#include <easylogging++.h>
#include "ace-selector.h"
#include "extravagant-conditions.h"

using namespace medici;

Cli::Cli(MixersFactory::MixerType mixer_type)
        : mixers_factory_(N), calculator_(mixers_factory_) {
    mixers_factory_.SetMixer(mixer_type);
    std::vector<uint_fast32_t> seeds;
    std::random_device rnd;
    for (size_t i = 0; i != 4; ++i) {
        seeds.push_back(rnd());
    }
    calculator_.SetRandomSeeds(seeds);
}

std::ostream& operator<<(std::ostream& s, const calculator::ExecutionParameters& params) {
    s << "found " << params.suitableDecks << " suitable decks out of "
      << params.checkedDecks;
    return s;
}

std::ostream& operator<<(std::ostream& s, const Card& card) {
    static std::map<uint_fast8_t, std::string> suits{
        {standard_36_deck::Suits::Clubs, "к"},
        {standard_36_deck::Suits::Spades, "п"},
        {standard_36_deck::Suits::Hearts, "ч"},
        {standard_36_deck::Suits::Diamonds, "б"},
    };
    static std::map<uint_fast8_t, std::string> ranks{
        {standard_36_deck::Ranks::Six, "6"},   {standard_36_deck::Ranks::Seven, "7"},
        {standard_36_deck::Ranks::Eight, "8"}, {standard_36_deck::Ranks::Nine, "9"},
        {standard_36_deck::Ranks::Ten, "X"},   {standard_36_deck::Ranks::Jack, "В"},
        {standard_36_deck::Ranks::Queen, "Д"}, {standard_36_deck::Ranks::King, "K"},
        {standard_36_deck::Ranks::Ace, "Т"},
    };
    s << ranks[card.rank] << suits[card.suit];
    (void)card.rank;
    return s;
}

std::ostream& operator<<(std::ostream& s, const std::vector<Card>& deck) {
    for (auto& card : deck) {
        s << " " << card << "";
    }
    return s;
}

std::ostream& operator<<(std::ostream& s, const std::set<Card>& deck) {
    for (auto& card : deck) {
        s << " " << card << "";
    }
    return s;
}

void Print(const std::vector<Card>& deck, medici::PatienceInfo info) {
    std::stringstream s;
    s << "<";
    for (size_t i = 0; i != deck.size(); ++i) {
        auto& card = deck[i];
        s << card;
        size_t convolutions = info.convolutions[card];
        if (convolutions != 0) {
            s << "(" << convolutions << ")";
            if (i == deck.size() - 1) {
                s << ">";
            } else {
                s << "> <";
            }
        } else {
            s << " ";
        }
    }
    LOG(INFO) << "Deck: " << s.str();
}

void Print(const std::vector<Card>& deck) {
    medici::PatienceInfo info;
    bool res = medici::TryToConverge(deck, info);
    LOG(INFO) << "Converges? " << (res ? "yes" : "no");
    if (res) {
        Print(deck, info);
        return;
    }
    LOG(INFO) << deck;
}

void Cli::Run() {
    using namespace standard_36_deck;
    static const size_t threads = 2;

    // // <Вп 9к Тп> <8б 6к Дк Дб Дп> <Кк Дч> <6б 6ч> <Кч 9ч> <9б 6п 8ч 9п> <Вб Вк 7ч Xк>
    // // <Xч> <7б Вч> <7к> <Тч> <8к> <7п Xп Xб 8п> <Тб> <Кп> <Тк> <Кб>
    // std::vector<Card> test{
    //     {Spades, Jack},  {Clubs, Nine},   {Spades, Ace},     {Diamonds, Eight},
    //     {Clubs, Six},    {Clubs, Queen},  {Diamonds, Queen}, {Spades, Queen},
    //     {Clubs, King},   {Hearts, Queen}, {Diamonds, Six},   {Hearts, Six},
    //     {Hearts, King},  {Hearts, Nine},  {Diamonds, Nine},  {Spades, Six},
    //     {Hearts, Eight}, {Spades, Nine},  {Diamonds, Jack},  {Clubs, Jack},
    //     {Hearts, Seven}, {Clubs, Ten},    {Hearts, Ten},     {Diamonds, Seven},
    //     {Hearts, Jack},  {Clubs, Seven},  {Hearts, Ace},     {Clubs, Eight},
    //     {Spades, Seven}, {Spades, Ten},   {Diamonds, Ten},   {Spades, Eight},
    //     {Diamonds, Ace}, {Spades, King},  {Clubs, Ace},      {Diamonds, King}};

    // // <Вп Кк 8к 7к> <Xп Вб 7ч Кб> <9ч> <6б> <Кч> <6к> <7б Вк> <7п> <Xк> <Дк Xб> <8ч
    // 8б>
    // // <6ч> <Дб> <8п Дч> <Tк Tб Tч> <Tп> <9б Кп> <Дп 6п> <Вч 9к Xч> <9п>
    // std::vector<Card> test{
    //     {Spades, Jack},    {Clubs, King},     {Clubs, Eight},  {Clubs, Seven},
    //     {Spades, Ten},     {Diamonds, Jack},  {Hearts, Seven}, {Diamonds, King},
    //     {Hearts, Nine},    {Diamonds, Six},   {Hearts, King},  {Clubs, Six},
    //     {Diamonds, Seven}, {Clubs, Jack},     {Spades, Seven}, {Clubs, Ten},
    //     {Clubs, Queen},    {Diamonds, Ten},   {Hearts, Eight}, {Diamonds, Eight},
    //     {Hearts, Six},     {Diamonds, Queen}, {Spades, Eight}, {Hearts, Queen},
    //     {Clubs, Ace},      {Diamonds, Ace},   {Hearts, Ace},   {Spades, Ace},
    //     {Diamonds, Nine},  {Spades, King},    {Spades, Queen}, {Spades, Six},
    //     {Hearts, Jack},    {Clubs, Nine},     {Hearts, Ten},   {Spades, Nine}};
    // Print(test);
    // return;

    DeckOneSelector first_card({AnySuit(Jack)}, 0, 0);
    DeckOneSelector second_card({AnySuit(Nine)}, 1, 1);
    DeckOneSelector third_card({AnySuit(Ten), AnySuit(Six)}, 2, 2);
    AceSelector ace_selector;

    DeckSelectors deck_selectors;
    deck_selectors.AddDeckSelector(first_card);
    deck_selectors.AddDeckSelector(second_card);
    deck_selectors.AddDeckSelector(third_card);
    deck_selectors.AddDeckSelector(ace_selector);

    PPatienceSelector patience_selector(new ExtravagantPatience());
    // PPatienceSelector patience_selector(new PatienceSelector);

    calculator::Manager::FoundVector all_decks;
    calculator_.Launch(threads, std::move(deck_selectors), std::move(patience_selector));
    while (all_decks.size() < 100) {
        LOG(INFO) << "Searching ...";
        std::this_thread::sleep_for(std::chrono::seconds(15));
        for (auto& params : calculator_.GetExecutionParameters()) {
            LOG(INFO) << params;
        }
        auto new_decks = calculator_.GetNewDecks();
        for (auto& pair : new_decks) {
            auto& deck = pair.first;
            auto& info = pair.second;
            Print(deck, info);
        }
        all_decks.insert(all_decks.end(), std::make_move_iterator(new_decks.begin()),
                         std::make_move_iterator(new_decks.end()));
    }
    calculator_.Interrupt();
    LOG(INFO) << "In total, found " << all_decks.size();
}

CardSelector Cli::AnySuit(standard_36_deck::Ranks rank, bool straight) {
    CardSelectorConfigurator configurator;
    configurator.Reset();
    configurator.SetRank(rank);
    configurator.SetStraight(straight);
    return configurator.GetSelector();
}
