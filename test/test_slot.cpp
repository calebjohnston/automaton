#include <memory>
#include <vector>

#include "gtest/gtest.h"

#include "slot.hpp"

TEST(Slot, DefaultConstructor) {
    class A { int a; };
    class B : A { int b; };
    using ptr_A = std::shared_ptr<A>;
    using ptr_B = std::shared_ptr<B>;

    ptr_A a1 = std::make_shared<A>( 10 );
    ptr_B b1 = std::make_shared<B>( 20 );
    std::vector<ptr_A> sample = { a1, b1 };

    Slot<A,A,0> sample_slot0(sample);
    Slot<B,A,1> sample_slot1(sample);

    EXPECT_EQ(10, sample_slot0.get());
    EXPECT_EQ(20, sample_slot1.get());
}

// Tests the c'tor that accepts a C string.
TEST(Slot, ConstructorFromCString) {
    const MyString s(kHelloString);
    EXPECT_EQ(0, strcmp(s.c_string(), kHelloString));
    EXPECT_EQ(sizeof(kHelloString)/sizeof(kHelloString[0]) - 1, s.Length());
}
