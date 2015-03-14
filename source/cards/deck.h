#ifndef CARDS_DECK_H
#define CARDS_DECK_H

#include <array>
#include <cstdint>

struct Card{
    struct Suit;
    struct Rank;

    struct Suit{
        template<typename T>
        Suit(typename std::enable_if<std::is_integral<T>::value, T>::type val) : Suit(static_cast<std::uint_fast8_t>(val)) {}
        Suit(std::uint_fast8_t);
        Suit(const Rank&) = delete;
        Suit();

        std::uint_fast8_t value;
        bool operator==(const Suit&) const;
        bool operator<(const Suit&) const;
    };

    struct Rank{
        template<typename T>
        Rank(typename std::enable_if<std::is_integral<T>::value, T>::type val) : Rank(static_cast<std::uint_fast8_t>(val)) {}
        Rank(std::uint_fast8_t);
        Rank(const Suit&) = delete;
        Rank();

        std::uint_fast8_t value;
        bool operator==(const Rank&) const;
        bool operator<(const Rank&) const;
    };

    Suit suit;
    Rank rank;
    bool operator<(const Card& other) const;
};

template<std::size_t n>
struct Deck{
    typedef std::array<Card, n> ArrayType;
    static const ArrayType cards;

    static constexpr std::size_t N() {return n;}
};

#endif /* CARDS_DECK_H */
