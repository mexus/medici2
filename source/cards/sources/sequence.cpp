#include <cards/sequence.h>

#include <algorithm>
#include <cassert>

#include <cards/storage.h>

namespace cards {

Sequence::Sequence() {}

Sequence::Sequence(const Sequence& other) : parts_(other.parts_) {}

Sequence::Sequence(Sequence&& other) : parts_(std::move(other.parts_)) {}

Sequence::Sequence(SimpleSequence&& sequence) {
    AddPart(std::move(sequence));
}

Sequence& Sequence::operator=(Sequence&& other) {
    parts_ = std::move(other.parts_);
    return *this;
}

void Sequence::AddPart(const SimpleSequence& other_part) {
    assert(!Contradicts(other_part) &&
           "Part should not contradict with the existent ones");
    for (auto& part : parts_) {
        if (part.Overlaps(other_part)) {
            part = part.Merge(other_part);
            return;
        }
    }
    parts_.push_back(other_part);
}

void Sequence::AddSequence(const Sequence& sequence) {
    assert(!Contradicts(sequence) && "Sequences should not be in contradiction");
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

const std::vector<SimpleSequence>& Sequence::GetParts() const {
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

size_t Sequence::OccupiedPlaces() const {
    size_t total = 0;
    for (auto& part: parts_) {
        total += part.cards.size();
    }
    return total;
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

bool Sequence::Contradicts(const SimpleSequence& other_part) const {
    for (auto& this_part : parts_) {
        if (!this_part.Overlaps(other_part)) {
            continue;
        }
        if (this_part.Contradicts(other_part)) {
            return true;
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
        if (it == rhs.parts_.end()) {
            return false;
        }
    }
    for (auto& part : rhs.parts_) {
        auto it = std::find(lhs.parts_.begin(), lhs.parts_.end(), part);
        if (it == lhs.parts_.end()) {
            return false;
        }
    }
    return true;
}
}
