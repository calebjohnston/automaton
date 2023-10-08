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
	ProgramCollection::Handle program_1 = programs.add(&sw);
	EXPECT_TRUE(programs.size() == 1);
	ProgramCollection::Handle program_2 = programs.make("xfer", "Network transfer", 10, 1, 1, Packet::Ping, Binary::Program, Encryption::Kerberos);
	EXPECT_TRUE(programs.size() == 2);
	
	EXPECT_TRUE(programs.remove(program_1));
	EXPECT_TRUE(programs.remove(program_2));
}
