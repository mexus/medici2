#include "sequence.h"

#include <algorithm>
#include <cassert>

#include "storage.h"

namespace cards {

bool ClosedSequence::Overlaps(const ClosedSequence& other) const {
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

ClosedSequence ClosedSequence::Merge(const ClosedSequence& other) const {
    assert(Overlaps(other) && "Sequences are not overlapping");
    ClosedSequence result;
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

bool ClosedSequence::Contradicts(const ClosedSequence& other) const {
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

bool ClosedSequence::IsOccupied(size_t place_number) const {
    if (place_number < position) {
        return false;
    }
    return place_number < position + cards.size();
}

Card ClosedSequence::GetCard(size_t place_number) const {
    assert(IsOccupied(place_number) && "Position is not occupied!");
    return cards[place_number - position];
}

bool operator==(const ClosedSequence& lhs, const ClosedSequence& rhs) {
    return lhs.position == rhs.position && lhs.cards == rhs.cards;
}

Sequence::Sequence() {}

Sequence::Sequence(const Sequence& other) : parts_(other.parts_) {}

Sequence::Sequence(Sequence&& other) : parts_(std::move(other.parts_)) {}

Sequence::Sequence(ClosedSequence&& sequence) {
    AddPart(std::move(sequence));
}

Sequence& Sequence::operator=(Sequence&& other) {
    parts_ = std::move(other.parts_);
    return *this;
}

void Sequence::AddPart(const ClosedSequence& sequence) {
    for (auto& part : parts_) {
        if (part.Overlaps(sequence)) {
            part = part.Merge(sequence);
            return;
        }
    }
    parts_.push_back(sequence);
}

void Sequence::AddPart(ClosedSequence&& sequence) {
    for (auto& part : parts_) {
        if (part.Overlaps(sequence)) {
            part = part.Merge(sequence);
            return;
        }
    }
    parts_.push_back(std::move(sequence));
}

void Sequence::AddSequence(const Sequence& sequence) {
    for (auto& part : sequence.parts_) {
        AddPart(part);
    }
}

bool Sequence::IsOccupied(size_t place_number) const {
    for (auto& part : parts_) {
        if (part.IsOccupied(place_number)) {
            return true;
        }
    }
    return false;
}

Card Sequence::GetCard(size_t place_number) const {
    for (auto& part : parts_) {
        if (part.IsOccupied(place_number)) {
            return part.GetCard(place_number);
        }
    }
    assert(false && "Position is not occupied!");
    /* In release build assertions are skipped, so this exception is needed to avoid
     * "control may reach end of non-void function" compilator warning */
    throw std::exception();
}

void Sequence::TakeCardsFromStorage(Storage* storage) const {
    for (auto& part : parts_) {
        for (auto& card : part.cards) {
            storage->TakeCard(card);
        }
    }
}

const std::vector<ClosedSequence>& Sequence::GetParts() const {
    return parts_;
}

std::vector<Sequence> Sequence::operator+(const std::vector<Sequence>& sequences) const {
    std::vector<Sequence> result;
    for (auto& sequence : sequences) {
        Sequence sequence_copy(*this);
        if (sequence_copy.Contradicts(sequence)) {
            continue;
        }
        sequence_copy.AddSequence(sequence);
        result.push_back(std::move(sequence_copy));
    }
    return result;
}

size_t Sequence::GetFirstOccupied() const {
    size_t min(0);
    bool init(false);
    for (auto& part : parts_) {
        size_t part_begin = part.position;
        if (!init) {
            min = part_begin;
            init = true;
            continue;
        }
        min = std::min(min, part_begin);
    }
    return min;
}

size_t Sequence::GetLastOccupied() const {
    size_t max(0);
    bool init(false);
    for (auto& part : parts_) {
        size_t part_end = part.position + part.cards.size() - 1;
        if (!init) {
            max = part_end;
            init = true;
            continue;
        }
        max = std::max(max, part_end);
    }
    return max;
}

bool Sequence::Contradicts(const Sequence& other) const {
    for (auto& this_part : parts_) {
        for (auto& other_part : other.parts_) {
            if (!this_part.Overlaps(other_part)) {
                continue;
            }
            if (this_part.Contradicts(other_part)) {
                return true;
            }
        }
    }
    return false;
}

Sequence Sequence::ShiftLeft(size_t shift) const {
    Sequence result;
    for (auto part : parts_) {
        if (part.position < shift) {
            size_t trim_size = shift - part.position;
            if (part.cards.size() <= trim_size) {
                // After trimming nothing remains
                continue;
            }
            part.cards.erase(part.cards.begin(), part.cards.begin() + trim_size);
            part.position = 0;
        } else {
            part.position -= shift;
        }
        result.AddPart(std::move(part));
    }
    return result;
}

Sequence Sequence::ShiftRight(size_t shift) const {
    Sequence result;
    for (auto part : parts_) {
        part.position += shift;
        result.AddPart(std::move(part));
    }
    return result;
}

bool operator==(const Sequence& lhs, const Sequence& rhs) {
    for (auto& part : lhs.parts_) {
        auto it = std::find(rhs.parts_.begin(), rhs.parts_.end(), part);
        if (it != rhs.parts_.end()) {
            return false;
        }
    }
    return true;
}
}
