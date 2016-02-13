#include <gtest/gtest.h>

#include <medici/patience-selector.h>
#include <medici/patience-target-card-selector.h>

TEST(Medici, PatienceSelector) {
    medici::PatienceSelector selector;
    ASSERT_TRUE(selector.Check({}, {}));
}

TEST(Medici, TargetCardSelector) {
    static const cards::Card test_card{0, 0};
    medici::PatienceTargetSelector selector(test_card);
    medici::PatienceInfo info;
    info.convolutions[test_card] = 1;
    ASSERT_TRUE(selector.Check({}, info));
}

TEST(Medici, TargetMaxSelector_strict) {
    static const bool strict_comparison = true;
    static const cards::Card test_card{0, 0};
    medici::PatienceMaxSelector selector(test_card, strict_comparison);

    medici::PatienceInfo info;
    info.convolutions[test_card] = 1;
    ASSERT_TRUE(selector.Check({}, info));
    ASSERT_FALSE(selector.Check({}, info));
    info.convolutions[test_card] = 2;
    ASSERT_TRUE(selector.Check({}, info));
}

TEST(Medici, TargetMaxSelector_not_strict) {
    static const bool strict_comparison = false;
    static const cards::Card test_card{0, 0};
    medici::PatienceMaxSelector selector(test_card, strict_comparison);

    medici::PatienceInfo info;
    info.convolutions[test_card] = 2;
    ASSERT_TRUE(selector.Check({}, info));
    ASSERT_TRUE(selector.Check({}, info));
    info.convolutions[test_card] = 1;
    ASSERT_FALSE(selector.Check({}, info));
    info.convolutions[test_card] = 3;
    ASSERT_TRUE(selector.Check({}, info));
}
