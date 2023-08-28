//
//  api.cpp
//
//  Created by Caleb Johnston on 8/26/2023.
//  Copyright © 2023 Caleb Johnston. All rights reserved.
//

#include <algorithm>
#include <chrono>
#include <deque>
#include <functional>
#include <iterator>
#include <memory>
#include <numeric>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <utility>

#include "api.h"
#include "game.h"

namespace Auto {

#pragma utility functions

template<typename T>
int index_for_name(const std::vector<T>& haystack, const std::string& name)
{
	int index = 0;
	for (const T& item : haystack) {
		if (item.name == name) return index;
		index++;
	}
	return -1;
}

#pragma misc API calls

Result test_api_1(Command& cmd)
{
	return list_programs(cmd.target->kernel).front();
}

Result test_api_2(Command& cmd)
{
	return { -1, "NOT IMPLEMENTED" };
}

Result test_api_3(Command& cmd)
{
	std::exit(EXIT_SUCCESS);
	return { 0, "doesn't matter" };
}

// TODO: requires a method to render the output
Result api_file_list(Command& cmd)
{
	the_game.mode_index = 0;

	return { -1, "NOT IMPLEMENTED" };
}

// TODO: requires a method to render the output
Result api_file_info(Command& cmd)
{
	the_game.mode_index = 0;
	
	return { -1, "NOT IMPLEMENTED" };
}

// TODO: requires a source and destination
Result api_copy_file(Command& cmd)
{
	the_game.mode_index = 0;
	
	return { -1, "NOT IMPLEMENTED" };
}

Result api_delete_file(Command& cmd)
{
	the_game.mode_index = 0;
	return delete_file(cmd.target->kernel, cmd.arguments.back());
}

Result api_uninstall_program(Command& cmd)
{
	the_game.mode_index = 0;
	return uninstall_program(cmd.target->kernel, cmd.arguments.back());
}

Result api_install_program(Command& cmd)
{
	the_game.mode_index = 0;
	return install_program(cmd.target->kernel, cmd.arguments.back());
}

Result api_not_implemented(Command& cmd)
{
	return { -1, "NOT IMPLEMENTED" };
}

#pragma transformation functions

// TODO: where do the SW properties come from when installing from a file?
Result install_program(Kernel& kernel, const std::string& name)
{
	int index = index_for_name<File>(kernel.files, name);
	if (index == -1) return { -1, "No file to install `" + name + "` from" };
	
	File& file = kernel.files[index];
	return install_program(kernel, {
		file.name,
		file.description,
		file.size,
		file.version,
		10,
		Packet::None,
		Binary::Program,
		Encryption::None
	});
}

Result uninstall_program(Kernel& kernel, const std::string& name)
{
	int index = index_for_name<Software>(kernel.programs, name);
	if (index == -1) return { -1, "no program found with name `" + name + "`" };
	
	return uninstall_program(kernel, index);
}

// TODO: where do the SW properties come from when installing from a file?
Result install_daemon(Kernel& kernel, const std::string& name)
{
	int index = index_for_name<File>(kernel.files, name);
	if (index == -1) return { -1, "No file to install `" + name + "` from" };
	
	File& file = kernel.files[index];
	return install_daemon(kernel, {
		file.name,
		file.description,
		file.size,
		file.version,
		10,
		Packet::None,
		Binary::Program,
		Encryption::None
	});
}

Result uninstall_daemon(Kernel& kernel, const std::string& name)
{
	int index = index_for_name<Software>(kernel.daemons, name);
	if (index == -1) return { -1, "no daemon found with name `" + name + "`" };
	
	return uninstall_daemon(kernel, index);
}

Result delete_file(Kernel& kernel, const std::string& name)
{
	int index = index_for_name<File>(kernel.files, name);
	if (index == -1) return { -1, "no file found with name `" + name + "`" };
	
	return delete_file(kernel, index);
}

}	// namespace Auto
