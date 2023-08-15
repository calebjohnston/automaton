#include <chrono>
#include <functional>
#include <string>
#include <thread>
#include <iostream>

#include "gtest/gtest.h"

using namespace std::placeholders;

TEST(DummyTests, TRUE_IS_TRUE)
{
	EXPECT_TRUE(true);
}
