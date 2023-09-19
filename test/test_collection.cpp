#include <chrono>
#include <functional>
#include <string>
#include <thread>
#include <iostream>

#include "gtest/gtest.h"

#include "model.h"
#include "collection.hpp"

TEST(CollectionTests, Construction)
{
	using namespace Auto;
	
	ProgramCollection programs;
	
	Software sw = { "ping", "ICMP network control", 10, 1, 1, Packet::Ping, Binary::Program, Encryption::Kerberos };
	ProgramCollection::Handle program_id = programs.add(&sw);
	EXPECT_TRUE(programs.size() == 1);
	bool result = programs.remove(program_id);
	EXPECT_TRUE(result);
}
