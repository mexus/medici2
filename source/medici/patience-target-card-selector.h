#pragma once
#include <atomic>
#include <mutex>

#include "patience-selector.h"

namespace medici {

class PatienceTargetSelector : public PatienceSelector {
public:
    PatienceTargetSelector(const Card& target);
    virtual ~PatienceTargetSelector() = default;

    virtual bool Check(const std::vector<Card>& deck, const PatienceInfo& info) override;

private:
    const Card target_;
};

class PatienceMaxSelector : public PatienceSelector {
public:
    PatienceMaxSelector(const Card& target, bool strict_comparison);

    virtual bool Check(const std::vector<Card>& deck, const PatienceInfo& info) override;

private:
    const Card target_;
    const bool strict_comparison_;
    std::mutex access_convolutions_;
    std::atomic<size_t> max_convolutions_;
};
}
