#pragma once
#include <cards/condition.h>
#include <medici/patience-selector.h>

class MediciCondition : public cards::Condition {
public:
    MediciCondition(size_t start_position, size_t end_position,
                    medici::PatienceSelector* selector);

    std::vector<cards::Sequence> GetVariants(const cards::Sequence& applied_sequence,
                                             cards::Storage storage) const override;
    bool CheckSequence(const std::vector<cards::Card>& cards) const override;

private:
    const size_t start_position_, end_position_;
    medici::PatienceSelector* selector_;

    static std::vector<cards::Card> GetPartialPermutation(std::vector<cards::Card> source,
                                                          size_t result_size,
                                                          uint64_t permutation);
    static uint64_t PermutationsCount(size_t source_size, size_t result_size);
    static bool ConvergesPreliminary(const std::vector<cards::Card>& deck);
};
