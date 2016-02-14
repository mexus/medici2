#include <cards/one-card-condition.h>

namespace cards {

OneCardCondition::OneCardCondition(size_t position, const Card& card, bool inverse)
        : position_(position), card_(card), inverse_(inverse) {}

bool OneCardCondition::CheckSequence(const std::vector<Card>& cards) const {
    if (position_ >= cards.size()) {
        return false;
    }
    auto& card = cards[position_];
    if (inverse_) {
        return !(card == card_);
    }
    return card == card_;
}

std::vector<Sequence> OneCardCondition::GetVariants(const Sequence& applied_sequence,
                                                    Storage storage) const {
    if (applied_sequence.IsOccupied(position_)) {
        if (!inverse_ && applied_sequence.GetCard(position_) != card_) {
            return {};
        }
        if (inverse_ && applied_sequence.GetCard(position_) == card_) {
            return {};
        }
        return {
            Sequence(SimpleSequence{{applied_sequence.GetCard(position_)}, position_})};
    }
    if (!inverse_) {
        if (!storage.HasCard(card_)) {
            return {};
        }
        return {Sequence(SimpleSequence{{card_}, position_})};
    }
    std::vector<Sequence> result;
    for (auto& card : storage.GetAllAvailableCards()) {
        result.emplace_back(SimpleSequence{{card}, position_});
    }
    return result;
}
}
