#include "medici-condition.h"

#include <algorithm>
#include <cassert>

#include <medici/patience.h>

using namespace cards;

MediciCondition::MediciCondition(size_t start_position, size_t end_position,
                                 medici::PatienceSelector* selector)
        : start_position_(start_position),
          end_position_(end_position),
          selector_(selector) {
    assert((end_position_ >= start_position_ + 2) &&
           "'end_position' should be at least 'start_position' + 2");
    assert(selector && "PatienceSelector should not be a null pointer");
}

bool MediciCondition::CheckSequence(const std::vector<Card>& cards) const {
    if (end_position_ >= cards.size()) {
        return false;
    }
    std::vector<Card> deck(cards.begin() + start_position_, cards.begin() + end_position_);
    medici::PatienceInfo info;
    if (!medici::TryToConverge(deck, info)) {
        return false;
    }
    return selector_->Check(deck, info);
}

std::vector<Sequence> MediciCondition::GetVariants(const Sequence& applied_sequence,
                                                   Storage storage) const {
    std::vector<Card> simple_sequence(end_position_ - start_position_ + 1);
    std::vector<size_t> free_positions;
    for (size_t position = start_position_; position <= end_position_; ++position) {
        if (applied_sequence.IsOccupied(position)) {
            simple_sequence[position - start_position_] =
                applied_sequence.GetCard(position);
            continue;
        }
        free_positions.push_back(position - start_position_);
    }
    if (free_positions.empty()) {
        medici::PatienceInfo info;
        if (!medici::TryToConverge(simple_sequence, info)) {
            return {};
        }
        if (!selector_->Check(simple_sequence, info)) {
            return {};
        }
        return {applied_sequence};
    }
    std::vector<Card> all_cards = storage.GetAllAvailableCards();
    if (all_cards.size() < free_positions.size()) {
        /* Not enough cards are available.
         * Actually this should never happen */
        return {};
    };
    std::vector<Sequence> result;
    size_t free_positions_count = free_positions.size();
    for (uint64_t permutation = 0,
                  end = PermutationsCount(all_cards.size(), free_positions_count);
         permutation != end; ++permutation) {
        std::vector<Card> free_cards =
            GetPartialPermutation(all_cards, free_positions_count, permutation);
        for (size_t i = 0; i != free_positions_count; ++i) {
            size_t position = free_positions[i];
            simple_sequence[position] = free_cards[i];
        }
        // if (ConvergesPreliminary(simple_sequence)) {
        //     continue;
        // }
        medici::PatienceInfo info;
        if (!medici::TryToConverge(simple_sequence, info)) {
            continue;
        }
        if (!selector_->Check(simple_sequence, info)) {
            continue;
        }
        result.emplace_back(SimpleSequence{simple_sequence, start_position_});
    }
    return result;
}

std::vector<Card> MediciCondition::GetPartialPermutation(std::vector<Card> source,
                                                         size_t result_size,
                                                         uint64_t permutation) {
    std::vector<Card> result;
    for (size_t card_number = 0; card_number != result_size; ++card_number) {
        size_t card_to_take = permutation % source.size();
        permutation /= source.size();
        auto it = source.begin() + card_to_take;
        result.push_back(*it);
        source.erase(it);
    }
    return result;
}

uint64_t MediciCondition::PermutationsCount(size_t source_size, size_t result_size) {
    assert((result_size <= source_size) &&
           "'result_size' should be less than or equal to 'source_size'");
    uint64_t res = 1;
    for (size_t i = source_size; i >= source_size - result_size + 1; --i) {
        if (std::numeric_limits<uint64_t>::max() / i < res) {
            throw std::logic_error("Result won't fit in uint64_t");
        }
        res *= i;
    }
    return res;
}

bool MediciCondition::ConvergesPreliminary(const std::vector<Card>& deck) {
    for (size_t pos = 2; pos < deck.size() - 1; ++pos) {
        const Card& left_card = deck[pos - 2];
        const Card& right_card = deck[pos];
        if (left_card.rank == right_card.rank || left_card.suit == right_card.suit) {
            return true;
        }
    }
    return false;
}
