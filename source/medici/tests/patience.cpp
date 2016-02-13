#include <gtest/gtest.h>

#include <cards/standard-36-deck.h>
#include <medici/patience.h>

namespace {
std::ostream& operator<<(::std::ostream& s, const cards::Card& card) {
    return s << "[" << (int)card.suit << ", " << (int)card.rank << "] ";
}
}

namespace {
std::set<cards::Card> GenerateMobiles(const std::vector<cards::Card>& cards,
                                      const std::set<cards::Card>& stationars) {
    std::set<cards::Card> result;
    for (auto& card : cards) {
        if (stationars.count(card) == 0) {
            result.insert(card);
        }
    }
    return result;
}
}

TEST(Medici, PatienceInfoClear) {
    medici::PatienceInfo info;
    info.convolutions[{0, 1}] = 10;
    info.mobiles.insert({1, 2});
    info.stationars.insert({2, 3});

    info.Clear();

    ASSERT_TRUE(info.convolutions.empty());
    ASSERT_TRUE(info.mobiles.empty());
    ASSERT_TRUE(info.stationars.empty());
}

using namespace cards;
using namespace standard_36_deck;

TEST(Medici, ConvergenceCase1) {
    std::vector<Card> converging_deck{
        {Jack, Spades},   {Nine, Diamonds},  {Ten, Spades},     {Queen, Diamonds},
        {Seven, Hearts},  {Seven, Diamonds}, {Queen, Spades},   {Jack, Hearts},
        {Ten, Diamonds},  {Jack, Clubs},     {Ten, Hearts},     {Seven, Spades},
        {Nine, Spades},   {Eight, Spades},   {King, Diamonds},  {Nine, Hearts},
        {Six, Hearts},    {Ace, Spades},     {Ace, Diamonds},   {Seven, Clubs},
        {Jack, Diamonds}, {Ace, Clubs},      {Eight, Hearts},   {Ace, Hearts},
        {Ten, Clubs},     {Six, Diamonds},   {Eight, Clubs},    {King, Spades},
        {King, Hearts},   {Nine, Clubs},     {Queen, Hearts},   {Six, Spades},
        {King, Clubs},    {Six, Clubs},      {Eight, Diamonds}, {Queen, Clubs}};
    medici::PatienceInfo expected_deck_info;
    expected_deck_info.stationars = {
        {Jack, Spades}, {Queen, Diamonds}, {Seven, Diamonds}, {Jack, Hearts},
        {Ten, Hearts},  {Seven, Spades},   {King, Diamonds},  {Six, Hearts},
        {Ace, Spades},  {Ace, Diamonds},   {Ace, Clubs},      {Ace, Hearts},
        {Ten, Clubs},   {Eight, Clubs},    {King, Hearts},    {Six, Spades},
        {Six, Clubs}};
    expected_deck_info.convolutions = {
        {{Ten, Hearts}, 1},     {{Queen, Hearts}, 2},  {{Ace, Hearts}, 10},
        {{Seven, Diamonds}, 1}, {{Jack, Diamonds}, 1}, {{Queen, Diamonds}, 1},
        {{Six, Clubs}, 6},      {{Eight, Clubs}, 1},   {{Jack, Clubs}, 3},
        {{Queen, Clubs}, 4},    {{Ace, Clubs}, 2},     {{Eight, Spades}, 1},
        {{Ten, Spades}, 1}};
    expected_deck_info.mobiles =
        GenerateMobiles(converging_deck, expected_deck_info.stationars);

    medici::PatienceInfo generated_info;
    ASSERT_TRUE(medici::TryToConverge(converging_deck, generated_info));
    ASSERT_EQ(expected_deck_info.stationars, generated_info.stationars);
    ASSERT_EQ(expected_deck_info.mobiles, generated_info.mobiles);
    ASSERT_EQ(expected_deck_info.convolutions, generated_info.convolutions);
}

TEST(Medici, ConvergenceCase2) {
    std::vector<Card> converging_deck{
        {Jack, Diamonds},  {Nine, Clubs},     {Ten, Hearts},    {Seven, Clubs},
        {Seven, Diamonds}, {Queen, Diamonds}, {Queen, Spades},  {Seven, Spades},
        {Jack, Hearts},    {King, Spades},    {Six, Spades},    {Eight, Spades},
        {Ace, Spades},     {Eight, Clubs},    {Ace, Clubs},     {Seven, Hearts},
        {Nine, Hearts},    {Ten, Clubs},      {Ace, Diamonds},  {Eight, Hearts},
        {Ace, Hearts},     {King, Clubs},     {Queen, Clubs},   {Queen, Hearts},
        {Ten, Spades},     {Nine, Spades},    {King, Hearts},   {Jack, Spades},
        {King, Diamonds},  {Six, Hearts},     {Nine, Diamonds}, {Eight, Diamonds},
        {Jack, Clubs},     {Ten, Diamonds},   {Six, Clubs},     {Six, Diamonds}};
    medici::PatienceInfo expected_deck_info;
    expected_deck_info.stationars = {
        {Jack, Diamonds}, {Nine, Clubs},   {Seven, Clubs},   {Seven, Diamonds},
        {Queen, Spades},  {Seven, Spades}, {King, Spades},   {Ace, Clubs},
        {Nine, Hearts},   {Ace, Diamonds}, {Ace, Hearts},    {King, Clubs},
        {Queen, Hearts},  {Nine, Spades},  {King, Diamonds}, {Nine, Diamonds},
        {Eight, Diamonds}};
    expected_deck_info.convolutions = {
        {{Ace, Hearts}, 10},  {{Six, Diamonds}, 1},  {{Nine, Diamonds}, 1},
        {{Ten, Diamonds}, 4}, {{King, Diamonds}, 1}, {{Six, Clubs}, 2},
        {{Seven, Clubs}, 1},  {{Eight, Clubs}, 1},   {{Ace, Clubs}, 1},
        {{Eight, Spades}, 1}, {{Jack, Spades}, 8},   {{King, Spades}, 2},
        {{Ace, Spades}, 1}};

    expected_deck_info.mobiles =
        GenerateMobiles(converging_deck, expected_deck_info.stationars);

    medici::PatienceInfo generated_info;
    ASSERT_TRUE(medici::TryToConverge(converging_deck, generated_info));
    ASSERT_EQ(expected_deck_info.stationars, generated_info.stationars);
    ASSERT_EQ(expected_deck_info.mobiles, generated_info.mobiles);
    ASSERT_EQ(expected_deck_info.convolutions, generated_info.convolutions);
}

TEST(Medici, NonConverging) {
    std::vector<Card> non_converging_deck{
        {Ten, Hearts},     {Eight, Clubs},    {Ace, Hearts},   {Jack, Clubs},
        {Seven, Spades},   {Ten, Clubs},      {Nine, Spades},  {Seven, Hearts},
        {Nine, Hearts},    {Ace, Diamonds},   {Six, Hearts},   {Queen, Spades},
        {Eight, Hearts},   {King, Diamonds},  {Ace, Spades},   {Ten, Spades},
        {Jack, Diamonds},  {Eight, Spades},   {Seven, Clubs},  {Nine, Diamonds},
        {Six, Spades},     {Queen, Hearts},   {King, Hearts},  {Jack, Spades},
        {Jack, Hearts},    {King, Spades},    {Nine, Clubs},   {Queen, Diamonds},
        {Ten, Diamonds},   {Eight, Diamonds}, {Six, Clubs},    {Queen, Clubs},
        {Seven, Diamonds}, {Ace, Clubs},      {Six, Diamonds}, {King, Clubs}};

    medici::PatienceInfo generated_info;
    ASSERT_FALSE(medici::TryToConverge(non_converging_deck, generated_info));
}
