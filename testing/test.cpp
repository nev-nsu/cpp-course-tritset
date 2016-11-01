#include <fstream>

#include "gtest/gtest.h"
#include "../Trit.h"
#include "../Tritset.h"

TEST (basic_tests, if_eq) {
    ASSERT_EQ(1, 1);
}

TEST (trit_tests, create) {
    ASSERT_EQ (Trit().value(), trit::Unknown);
    ASSERT_EQ (Trit(trit::True).value(), trit::True);
    Trit tmp(false);
    ASSERT_EQ (Trit(tmp).value(), trit::False);
}

TEST (trit_tests, cast) {
    ASSERT_FALSE (bool (Trit(false)));
    ASSERT_TRUE (bool (Trit(true)));
    ASSERT_EQ (trit(Trit(false)), trit::False);
}

TEST (trit_tests, compare) {
    Trit t(true), f(false), u(trit::Unknown);
    ASSERT_TRUE(t == t);
    ASSERT_TRUE(f == f);
    ASSERT_TRUE(u == u);
    ASSERT_TRUE(t != f);
    ASSERT_TRUE(u != t);

}

TEST (trit_tests, logical_and) {
    Trit t(true), f(false), u(trit::Unknown);
    ASSERT_EQ (t & t, t);
    ASSERT_EQ (t & f, f);
    ASSERT_EQ (f & f, f);
    ASSERT_EQ (f & t, f);
    ASSERT_EQ (f & u, f);
    ASSERT_EQ (u & t, u);
}

TEST (trit_tests, logical_or) {
    Trit t(true), f(false), u(trit::Unknown);
    ASSERT_EQ (t | t, t);
    ASSERT_EQ (t | f, t);
    ASSERT_EQ (f | f, f);
    ASSERT_EQ (f | t, t);
    ASSERT_EQ (f | u, u);
    ASSERT_EQ (u | t, t);
}

TEST (trit_tests, logical_xor) {
    Trit t(true), f(false), u(trit::Unknown);
    ASSERT_EQ (t ^ t, f);
    ASSERT_EQ (t ^ f, t);
    ASSERT_EQ (f ^ f, f);
    ASSERT_EQ (f ^ t, t);
    ASSERT_EQ (f ^ u, u);
    ASSERT_EQ (u ^ t, u);
}

TEST (trit_tests, logical_not) {
    Trit t(true), f(false), u(trit::Unknown);
    ASSERT_EQ(!t, f);
    ASSERT_EQ(!f, t);
    ASSERT_EQ(!u, u);
}

TEST (trit_tests, assign) {
    Trit tmp(true);
    tmp &= Trit(false);
    ASSERT_EQ (tmp.value(), trit::False);
    tmp |= Trit(false);
    ASSERT_EQ (tmp.value(), trit::False);
    tmp ^= Trit(true);
    ASSERT_EQ (tmp.value(), trit::True);
    tmp = Trit();
    ASSERT_EQ (tmp.value(), trit::Unknown);
}

TEST (tritset_tests, printf) {
    Tritset set(10, Trit(true));
    std::string str = set.to_string();
    ASSERT_EQ(str, "1111111111??????");
}

TEST (tritset_tests, constructors) {
    Tritset set(20, Trit(true));
    ASSERT_EQ(set.to_string(), "11111111111111111111????????????");
    Tritset set2(10, Trit(true));
    ASSERT_EQ(set2.to_string(), "1111111111??????");
    Tritset set3(10);
    ASSERT_EQ(set3.to_string(), "????????????????");
    std::vector<int> a{-1, 1, 0, 1, 0, 1}, b{-1, -1, 0, 1};
    Tritset set4(a), set5(b);
    ASSERT_EQ (set4.to_string(), "01?1?1??????????");
    ASSERT_EQ (set5.to_string(), "00?1????????????");
}

TEST (tritset_tests, counters_length) {
    ASSERT_EQ(Tritset(20, Trit(true)).length(), 20);
    ASSERT_EQ(Tritset(10, Trit(false)).length(), 10);
    ASSERT_EQ(Tritset(10).length(), 0);
}

TEST (tritset_tests, counters_capacity) {
    ASSERT_EQ(Tritset(20, Trit(true)).capacity(), 32);
    ASSERT_EQ(Tritset(10, Trit(false)).capacity(), 16);
    ASSERT_EQ(Tritset(10).capacity(), 16);
}

TEST (tritset_tests, counters_count) {
    Tritset set(20, Trit(true));
    ASSERT_EQ(set.count(Trit(true)), 20);
    ASSERT_EQ(set.count(Trit(false)), 0);
    ASSERT_EQ(set.count(Trit()), 12);
}

TEST (tritset_tests, counters_size) {
    ASSERT_EQ(Tritset(20, Trit(true)).size(), 2);
    ASSERT_EQ(Tritset(10, Trit(false)).size(), 1);
    ASSERT_EQ(Tritset(10).size(), 1);
}

TEST (tritset_tests, assignments) {
    Tritset set(20, Trit(true));
    set[0] = Trit(false);
    ASSERT_EQ(set.to_string(), "01111111111111111111????????????");
    set[0] = Trit();
    ASSERT_EQ(set.to_string(), "?1111111111111111111????????????");
    set[31] = Trit();
    ASSERT_EQ(set.length(), 20);
    set[31] = Trit(true);
    ASSERT_EQ(set.length(), 32);
    set[32] = Trit();
    ASSERT_EQ(set.capacity(), 32);
    set[32] = Trit(true);
    ASSERT_EQ(set.capacity(), 48);
}

TEST (tritset_tests, resize_shrink) {
    Tritset set(40);
    set[2] = true;
    set.shrink();
    ASSERT_EQ(set.capacity(), 16);
    set[20] = true;
    set.shrink();
    ASSERT_EQ(set.capacity(), 32);
}


TEST (tritset_tests, resize_cut) {
    Tritset set(40);
    set[2] = set[20] = true;
    set.cut(3);
    ASSERT_EQ(set.capacity(), 16);
    ASSERT_EQ(set.length(), 3);
    ASSERT_EQ(set.to_string(), "??1?????????????");
}


TEST (tritset_tests, resize_extend) {
    Tritset set(40, true);
    set.extend(60);
    ASSERT_EQ(set.capacity(), 64);
    ASSERT_EQ(set.length(), 40);
    ASSERT_EQ (set.count(Trit(true)), 40);
}

TEST (tritset_tests, simple_test) {
    Tritset set(20, true);
    for (int i = 0; i < 20; i += 2)
        set[i] = false;
    ASSERT_EQ (set.to_string(), "01010101010101010101????????????");
    for (int i = 0; i < set.length(); i++)
        if (set[i] == -1 && i % 4 == 0)
            set[i] = trit::Unknown;
    ASSERT_EQ (set.to_string(), "?101?101?101?101?101????????????");
}

TEST (tritset_tests, logical) {
    std::vector<int> a = {-1, 1, 0, 1, 0, 1}, b = {-1, -1, 0, 1};
    Tritset set(a), set2(b);
    ASSERT_EQ ((set & set2).to_string(), "00?1????????????");
    ASSERT_EQ ((set | set2).to_string(), "01?1?1??????????");
    ASSERT_EQ ((set ^ set2).to_string(), "01?0????????????");
    ASSERT_EQ ((set | set2).length(), 6);
}

TEST (tritset_tests, iterator) {
    std::vector<int> a = {-1, 1, 0, 1, 0, 1}, b;
    Tritset set(a);
    for (auto i : set)
        b.push_back(int(i));
    ASSERT_EQ (a, b);
}

TEST (tritset_tests, const_tritset) {
    std::vector<int> a = {-1, 1, 0, 1, 0, 1};
    const Tritset set(a);
    ASSERT_EQ (int(set[2]), a[2]);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}