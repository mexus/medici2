#include <cards/any-rank-condition.h>

namespace cards {

AnyRankCondition::AnyRankCondition(size_t position, uint_fast8_t suit, bool inverse)
        : position_(position), suit_(suit), inverse_(inverse) {}

bool AnyRankCondition::CheckSequence(const std::vector<Card>& cards) const {
    if (position_ >= cards.size()) {
        return false;
    }
    if (inverse_) {
        return cards[position_].suit != suit_;
    }
    return cards[position_].suit == suit_;
}

std::vector<Sequence> AnyRankCondition::GetVariants(const Sequence& applied_sequence,
                                                    Storage storage) const {
    if (applied_sequence.IsOccupied(position_)) {
        if (!inverse_ && applied_sequence.GetCard(position_).suit != suit_) {
            return {};
        }
        if (inverse_ && applied_sequence.GetCard(position_).suit == suit_) {
            return {};
        }
        return {
            Sequence(SimpleSequence{{applied_sequence.GetCard(position_)}, position_})};
    }
    std::vector<Card> cards;
    if (!inverse_) {
        for (auto& rank : storage.GetRanksForSuit(suit_)) {
            cards.push_back(Card{suit_, rank});
        }
    } else {
        for (auto& card : storage.GetAllAvailableCards()) {
            if (card.suit != suit_) {
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
