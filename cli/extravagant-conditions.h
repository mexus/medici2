#include <medici/patience-selector.h>

class ExtravagantPatience : public medici::PatienceSelector {
public:
    bool Check(const std::vector<Card>& deck, const medici::PatienceInfo& info) override;

private:
    static bool IsAce(const Card& card);
    static size_t GetConvolutions(const std::map<Card, size_t>& convolutions,
                                  const Card& card);
};
