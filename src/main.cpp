//
//  main.cpp
//
//  Created by Caleb Johnston on 11/16/2020.
//  Copyright © 2020 Caleb Johnston. All rights reserved.
//

#include <cstring>
#include <deque>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

#include "computer.h"
#include "kernel.h"
#include "network.h"

//---------------------
#include <variant>
#include "composition.hpp"

auto twice = [](const auto& f)
{
    return [&f](int x) {
        return f(f(x));
    };
};

auto plus_three = [](int i)
{
    return i + 3;
};

void test_hof()
{
    auto g = twice(plus_three);

    std::cout << "g(7) == " << g(7) << std::endl; // 13
}

//---------------------

// STRUCTURE GUIDELINES
//---------------------
// create normal OOP type hierarchy
	// base types must not have pure virtual members
	// POD types have all public members
	// classes with private members must completely OWN and totally HIDE those members
// create comparators and null evaluator for each hierarchical base type
// create class factory constructors for each type hierarchy
// create variant collections for each hierarchical base type
// manage variant collections with composable functions
// create networks of variant collections using composable functions + shared_ptrs
// query networks of variant collections using composable functions

// CLI DESIGN NOTES
//---------------------
// Treat each program as processing its own inputs (as I have done below)
// Create abstraction for this input processing that works for all programs
// Process CLI input by finding the right program and supplying it the inputs
// Take program outputs and process those through the host machine properties

void create_world()
{
	// attempt 2
	/*
	 auto cpu = MakeComponent(...bunch of details...)
	 auto disk = MakeComponent(...bunch of details...)
	 auto computer = MakeComputer(cpu, disk, ...)

	 auto program = MakeProgram(...bunch of details...)
	 auto daemon = MakeDaemon(...bunch of details...)
	 auto file = MakeDaemon(...bunch of details...)
	 auto system = MakeSystem(...bunch of details...)
	 system.add(program)
	 system.add(daemon)
	 system.add(file)

	 auto host1 = MakeHost( name, descr, computer1, system1 );
	 auto host2 = MakeHost( name, descr, computer2, system2 );

	 auto network = MakeNetwork( name, descr )
	 network.add(host1)
	 network.add(host2)
	*/

	// attempt 3
	/*

	 cpu > disk >

	 */

	 // METHOD 1
	 KernelRef test_computer1 = KernelRef(new Kernel("comp1"));

	 test_computer1 | setTest10 | addTest10;

	 std::cout << "case: " + test_computer1->name() + ".test = " << test_computer1->test() << std::endl;

	 KernelFunction infosec = operator|(applySecurity, policy); // works
	 // KernelFunction infosec = applySecurity | policy; // does not work 🤔

	 KernelRef test_computer2 = KernelRef(new Kernel("comp2"));
	 KernelRef test_computer3 = KernelRef(new Kernel("comp3"));
	 test_computer3->addAgents();
	 std::vector<KernelRef> input = { test_computer1, test_computer2, test_computer3 };
	 std::vector<KernelRef> output;
	 std::transform(input.begin(), input.end(), std::back_inserter(output), infosec);
	 std::for_each(output.begin(), output.end(), [](auto kernel){ std::cout << kernel->name() << ": " << kernel->test() << std::endl; });

	 // METHOD 2
	 // KernelRef test_computer2 = KernelRef(new Kernel("comp2"));
	 // MaybeKernelRef maybe_computer = test_computer2;
	 //
	 // maybe_computer | setTest10 | subtractTest20 | addTest10 | addTest10;
	 //
 	 // std::cout << "case: " + std::get<KernelRef>(maybe_computer)->name() + ".test = " << std::get<KernelRef>(maybe_computer)->test() << std::endl;

}

int main(int argc, const char * argv[])
{
// test_hof();
create_world();

	// collect command line arguments
	std::deque<std::string> commandLineArgs;
	for (int arg = 0; arg < argc; ++arg ) {
		commandLineArgs.push_back( std::string( argv[arg] ) );
	}

	// verify command line inputs
	if (commandLineArgs.size() < 3) {
		std::cout << "Usage: " << commandLineArgs.at(0) << " [input csv] [output location] (OPTIONS)" << std::endl;
		std::cout << " Optional parameters:" << std::endl;
		std::cout << "    -h  ignore header of CSV file" << std::endl;
		std::cout << "    -m  collect metrics on input data" << std::endl;
		std::cout << "    -b  specify number of records per file; enables file output batching" << std::endl;
		return -1;
	}

	// parse command line inputs
	commandLineArgs.pop_front(); // skip program name
	std::string input_filepath = commandLineArgs.front(); commandLineArgs.pop_front();
	std::string output_location = commandLineArgs.front(); commandLineArgs.pop_front();
	char last_char = output_location.back();
	if (0 != std::strncmp("/", &last_char, 1)) {
		output_location += "/";
	}

	bool ignore_header = false;
	bool collect_metrics = false;
	int batch_size = 0;
	while (!commandLineArgs.empty()) {
		std::string arg = commandLineArgs.front();
		if ("-h" == arg) {
			ignore_header = true;
		}
		else if ("-m" == arg) {
			collect_metrics = true;
		}
		else if ("-b" == arg) {
			commandLineArgs.pop_front();
			std::string param = commandLineArgs.front();
			try {
				batch_size = std::stoi( param );
			}
			catch ( ... ) {
				std::cout << "Failed to parse batch size. " << arg << std::endl;
				return -1;
			}
		}
		else {
			std::cout << "ignoring invalid argument: " << arg << std::endl;
		}
		commandLineArgs.pop_front();
	}

    return 0;
}
