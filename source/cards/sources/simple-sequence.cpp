#include <cards/simple-sequence.h>

#include <cassert>

namespace cards {

bool SimpleSequence::Overlaps(const SimpleSequence& other) const {
    auto& x = *this;
    auto& y = other;
    /* If sequences are 'touching' each other, we consider them as overlapping */
    if (x.position > y.position + y.cards.size()) {
        /* The beginning of 'x' is after the end of 'y' */
        return false;
    }
    if (x.position + x.cards.size() < y.position) {
        /* The end of 'x' is before the beginning of 'y'
         * This condition can be derived from the first one with a substitution
         * 'x' <-> 'y' */
        return false;
    }
    return true;
}

SimpleSequence SimpleSequence::Merge(const SimpleSequence& other) const {
    assert(Overlaps(other) && "Sequences are not overlapping");
    assert(!Contradicts(other) && "Sequences should not be in contradiction");
    SimpleSequence result;
    auto& x = *this;
    auto& y = other;
    result.position = std::min(x.position, y.position);
    for (size_t i = result.position,
                end = std::max(x.position + x.cards.size(), y.position + y.cards.size());
         i != end; ++i) {
        if (i >= x.position && i < x.position + x.cards.size()) {
            result.cards.push_back(x.cards[i - x.position]);
        } else {
            result.cards.push_back(y.cards[i - y.position]);
        }
    }
    return result;
}

bool SimpleSequence::Contradicts(const SimpleSequence& other) const {
    auto& x = *this;
    auto& y = other;
    size_t first = std::max(x.position, y.position);
    size_t last = std::min(x.position + x.cards.size(), y.position + y.cards.size());
    for (size_t i = first; i < last; ++i) {
        if (x.GetCard(i) != y.GetCard(i)) {
            return true;
        }
    }
    return false;
}

bool SimpleSequence::IsOccupied(size_t place_number) const {
    if (place_number < position) {
        return false;
    }
    return place_number < position + cards.size();
}

Card SimpleSequence::GetCard(size_t place_number) const {
    assert(IsOccupied(place_number) && "Position is not occupied!");
    return cards[place_number - position];
}

bool operator==(const SimpleSequence& lhs, const SimpleSequence& rhs) {
    return lhs.position == rhs.position && lhs.cards == rhs.cards;
}

}
