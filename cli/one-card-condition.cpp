#include "one-card-condition.h"

OneCardCondition::OneCardCondition(size_t position, const Card& card, bool inverse)
        : position_(position), card_(card), inverse_(inverse) {}

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
            Sequence(ClosedSequence{{applied_sequence.GetCard(position_)}, position_})};
    }
    if (!inverse_) {
        if (!storage.HasCard(card_)) {
            return {};
        }
        return {Sequence(ClosedSequence{{card_}, position_})};
    }
    std::vector<Sequence> result;
    for (auto& card : storage.GetAllAvailableCards()) {
        result.emplace_back(ClosedSequence{{card}, position_});
    }
    return result;
}

AnyCardCondition::AnyCardCondition(size_t position) : position_(position) {}

std::vector<Sequence> AnyCardCondition::GetVariants(const Sequence& applied_sequence,
                                                    Storage storage) const {
    if (applied_sequence.IsOccupied(position_)) {
        return {
            Sequence(ClosedSequence{{applied_sequence.GetCard(position_)}, position_})};
    }
    std::vector<Sequence> result;
    for (auto& card : storage.GetAllAvailableCards()) {
        result.emplace_back(ClosedSequence{{card}, position_});
    }
    return result;
}

AnyRankCardCondition::AnyRankCardCondition(size_t position, uint_fast8_t suit,
                                           bool inverse)
        : position_(position), suit_(suit), inverse_(inverse) {}

std::vector<Sequence> AnyRankCardCondition::GetVariants(const Sequence& applied_sequence,
                                                        Storage storage) const {
    if (applied_sequence.IsOccupied(position_)) {
        if (!inverse_ && applied_sequence.GetCard(position_).suit != suit_) {
            return {};
        }
        if (inverse_ && applied_sequence.GetCard(position_).suit == suit_) {
            return {};
        }
        return {
            Sequence(ClosedSequence{{applied_sequence.GetCard(position_)}, position_})};
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
        result.emplace_back(ClosedSequence{{card}, position_});
    }
    return result;
}

AnySuitCardCondition::AnySuitCardCondition(size_t position, uint_fast8_t rank,
                                           bool inverse)
        : position_(position), rank_(rank), inverse_(inverse) {}

std::vector<Sequence> AnySuitCardCondition::GetVariants(const Sequence& applied_sequence,
                                                        Storage storage) const {
    if (applied_sequence.IsOccupied(position_)) {
        if (!inverse_ && applied_sequence.GetCard(position_).rank != rank_) {
            return {};
        }
        if (inverse_ && applied_sequence.GetCard(position_).rank == rank_) {
            return {};
        }
        return {
            Sequence(ClosedSequence{{applied_sequence.GetCard(position_)}, position_})};
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
        result.emplace_back(ClosedSequence{{card}, position_});
    }
    return result;
}
