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
#include <map>

#include "computer.h"
#include "kernel.h"
#include "network.h"
#include "node.h"

#include "model.h"
#include "game.h"

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

int main(int argc, const char * argv[])
{
//	typedef std::vector<std::string> Arguments;
//	typedef std::map<std::string,Arguments> FunctionMap;
//	typedef std::map<std::string,FunctionMap> CommandMap;
	
	
	Auto::load_gamestate();
	Auto::gameplay_loop();
	
	return 0;
	
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
