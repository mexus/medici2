#pragma once
#include "condition.h"
#include <medici/patience-selector.h>

class MediciCondition : public Condition {
public:
    MediciCondition(size_t start_position, size_t end_position,
                    medici::PatienceSelector* selector);

    std::vector<Sequence> GetVariants(const Sequence& applied_sequence,
                                      Storage storage) const override;
    bool CheckSequence(const std::vector<Card>& cards) const override;

private:
    const size_t start_position_, end_position_;
    medici::PatienceSelector* selector_;

    static std::vector<Card> GetPartialPermutation(std::vector<Card> source,
                                                   size_t result_size,
                                                   uint64_t permutation);
    static uint64_t PermutationsCount(size_t source_size, size_t result_size);
    static bool ConvergesPreliminary(const std::vector<Card>& deck);
};
