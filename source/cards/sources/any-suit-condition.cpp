#include <cards/any-suit-condition.h>

namespace cards {

AnySuitCondition::AnySuitCondition(size_t position, uint_fast8_t rank, bool inverse)
        : position_(position), rank_(rank), inverse_(inverse) {}

bool AnySuitCondition::CheckSequence(const std::vector<Card>& cards) const {
    if (position_ >= cards.size()) {
        return false;
    }
    if (inverse_) {
        return cards[position_].rank != rank_;
    }
    return cards[position_].rank == rank_;
}

std::vector<Sequence> AnySuitCondition::GetVariants(const Sequence& applied_sequence,
                                                    Storage storage) const {
    if (applied_sequence.IsOccupied(position_)) {
        if (!inverse_ && applied_sequence.GetCard(position_).rank != rank_) {
            return {};
        }
        if (inverse_ && applied_sequence.GetCard(position_).rank == rank_) {
            return {};
        }
        return {
            Sequence(SimpleSequence{{applied_sequence.GetCard(position_)}, position_})};
    }
    std::vector<Card> cards;
    if (!inverse_) {
        for (auto& suit : storage.GetSuitsForRank(rank_)) {
            cards.push_back(Card{suit, rank_});
        }
    } else {
        for (auto& card : storage.GetAllAvailableCards()) {
            if (card.rank != rank_) {
                cards.push_back(card);
            }
        }
    }
    std::vector<Sequence> result;
    for (auto& card : cards) {
        result.emplace_back(SimpleSequence{{card}, position_});
    }
    return result;
}
}
