//
//  main.cpp
//
//  Created by Caleb Johnston on 11/16/2020.
//  Copyright © 2020 Caleb Johnston. All rights reserved.
//

#include <cstring>
#include <deque>
#include <iostream>
#include <memory>
#include <string>

#include "computer.h"
#include "kernel.h"
#include "network.hpp"

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
/*
enum EnumT1 {
	Active,
	Inactive,
	Busy,
	None
};

class DataT1 {
	std::string prop1;
	float prop2;
	int prop3;
	EnumT1 state;
	
	
};

struct DataT2 : public DataT1 {
	std::string prop3;
	std::string prop4;
//	std::vector<int> nums;
};
*/

void create_world()
{
//	DataT1 obj1 = { "prop1", 1.0f, 1, Active };
//	DataT2 obj2 = { "prop3", "prop4", "prop1", 0.0f, 1, Active };
	
	// attempt 1
	Component cpu;
	Component disk;
	Component memory;
	Component nic;
	Device battery;
	Device power;
	Computer host = { cpu, disk, memory, nic, battery, power };
	
//	std::string name;
//	std::string description;
//	std::vector<Daemon> daemons;
//	std::vector<Program> programs;
//	std::vector<Data> filesystem;
	Kernel sys = { "name", "desc" };
	
	HostRef node = std::make_shared<Host>();
	node->name = "name";
	node->description = "desc";
	node->machine = host;
	node->system = sys;
	
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
}

int main(int argc, const char * argv[])
{
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

