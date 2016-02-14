#pragma once
#include <atomic>

#include <cards/condition.h>
#include <cards/standard-36-deck.h>
#include <helpers/sequence-mixer.h>
#include <mixer/factory.h>

class Performance {
public:
    Performance();
    void Run(MixersFactory::MixerType);

private:
    static constexpr std::size_t N = 36;
    MixersFactory::MixerType mixer_type_;
    MixersFactory mixers_factory_36_;
    static std::vector<cards::Sequence> default_sequences_;

    void Mixing() const;
    void ConditionGenerator() const;

    std::vector<std::vector<cards::Card>> PregenerateConvergableDecks() const;
    void MediciGenerator() const;
    void MediciWithConditions() const;
    void MediciWithConditionsAndIChing() const;
    void IChingBalancedPercent() const;

    static std::shared_ptr<cards::Condition> DefaultPreCondition();
    static std::shared_ptr<cards::Condition> DefaultPostCondition();

    std::shared_ptr<MixerInterface<cards::Card>> GetMixer36(
        std::uint_fast32_t seed = 0) const;
};
