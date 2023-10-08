//
//  model.h
//
//  Created by Caleb Johnston on 8/19/2023.
//  Copyright © 2023 Caleb Johnston. All rights reserved.
//

#pragma once

#include <functional>
#include <iterator>
#include <memory>
#include <numeric>
#include <string>
#include <vector>

/*
 * Architecture and design considerations:
 * 	- Refactor the POD types to support handles
 *	- POD types could benefit from convenience functions for comparators, string IO, (de)serialization support, etc.
 * 	- POD types might need an umambiguous way to be empty or null which is currently ill defined
 *		- consider the scenario in which you want to determine if a specific instance of software is already installed
 *	- POD types might have nasty memory allocation issues (what is memory layout for vector<Computer>?)
 *	- POD types don't provide base types which are required for writing generic (aka DRY) code
 *	- A class factory pattern that vends typesafe ID handles will be required to perform reliable conversion from commands to actions
 *		- its also required so that you can maintain references to targets that are passed between scopes
 *		- furthermore, the class factories should maintain a table/list of objects where each object stores its own lookup ID (maybe?)
 *	- The Result and ResultSet types need to be revisited because they mix two semantics: internal mutation status, and user query results
 *	- FTXUI doesn't appear to support disabling/enabling components. Might be required for me though..
 *	- With the default output going to stdio, I'll need a file logger to get actual log output
 *	- I've hit my limit with jamming everything into a few files. References are getting hard to keep sequential
 *	- I don't have a very simple method of handling invalid user input. I rely upon an "InvalidAction" type that works but is inelegant.
 *	- FTXUI can only animate Components, not Elements. This makes animating regular output impossible.
 *	- FTXUI cannot render string output without a constant string reference. Sometimes this is inconvenient.
 * 		- This CAN be mitigated by wrapping a component in a renderer that captures the input fields as part of the closure scope
 *		- Could there be a global DOM that is safely manipulated without introducing defects? (edge case: dynamically generated fields)
 *		- Could it be mitigated by recreating views when the mode changes?
 *	- If I don't want the user to consume a whole turn with an invalid command, then I'll need to have a validate_command() functions for every API operation that can return an error message
 */

namespace Auto {

enum class Packet {
	Ping,
	Inference,
	Reflection,
	Symplex,
	Autoregression,
	None
};
static std::string to_str(Packet p) {
	switch (p) {
		case Packet::Ping: return "Ping";
		case Packet::Inference: return "Inference";
		case Packet::Reflection: return "Reflection";
		case Packet::Symplex: return "Symplex";
		case Packet::Autoregression: return "Autoregression";
		default: return "None";
	}
}

enum class Encryption {
	DH4,
	X509,
	Kerberos,
	None
};

enum class Component {
	Disk,
	Memory,
	Processor,
	Network,
	Battery,
	Power,
	None
};
static std::string to_str(Component c) {
	switch (c) {
		case Component::Disk: return "Disk";
		case Component::Memory: return "Memory";
		case Component::Processor: return "Processor";
		case Component::Network: return "Network";
		case Component::Battery: return "Battery";
		case Component::Power: return "Power";
		default: return "None";
	}
}

enum class Binary {
	Program,
	Daemon
};

enum class Status {
	Active,
	Inactive,
	Busy,
	None
};
static std::string to_str(Status s) {
	switch (s) {
		case Status::Active: return "Active";
		case Status::Inactive: return "Inactive";
		case Status::Busy: return "Busy";
		default: return "None";
	}
}


enum class Class {
	Electronics,
	Monitor,
	Sentry,
	Worm,
	Virus,
	Simulation,
	Agent,
	Automaton,
	Player,
	None
};
static std::string to_str(Class c) {
	switch (c) {
		case Class::Electronics: return "Electronics";
		case Class::Monitor: return "Monitor";
		case Class::Sentry: return "Sentry";
		case Class::Worm: return "Worm";
		case Class::Virus: return "Virus";
		case Class::Simulation: return "Simulation";
		case Class::Agent: return "Agent";
		case Class::Automaton: return "Automaton";
		case Class::Player: return "Human";
		default: return "None";
	}
}

class File {
public:
	File() = default;
	File(std::string pname,
		 std::string pdesc,
		 int pver,
		 int psize)
	  :	name(pname),
		description(pdesc),
		version(pver),
		size(psize) {}
	
	std::string name;
	std::string description;
	int version;
	int size;
};

class Software : public File {
public:
	Software() = default;
	Software(std::string pname,
			 std::string pdesc,
			 int pver,
			 int psize,
			 int pcycles,
			 Packet pdata,
			 Binary pexec,
			 Encryption pcrypto)
	  :	File(pname, pdesc, pver, psize),
		cycles(pcycles),
		data_type(pdata),
		exec_type(pexec),
		crypto(pcrypto) {}
	
	int cycles;
	Packet data_type;
	Binary exec_type;
	Encryption crypto;
};

struct Device {
	std::string name;
	Component type;
	int capacity;
	int power;
};

struct Computer {
	std::string serial;
	std::string manufacturer;
	Device disk;
	Device memory;
	Device processor;
	Device uplink;
	Device battery;
	Device power_unit;
};

struct Kernel {
	std::string hostname;
	std::vector<Software> programs;
	std::vector<Software> daemons;
	std::vector<File> files;
	std::vector<Kernel> connections;
	int version;
	Computer computer;
	int hitpoints;
};

struct Agent {
	std::string name;
	std::string description;
	Status status;
	Class type;
	int version;
	Kernel kernel;
};

struct Result {
	int status;
	std::string message;
};

typedef std::vector<Result> ResultSet;

class Command {
public:
	std::string function;
	Agent* target;
	std::vector<std::string> arguments;
};

#pragma utility functions

int computer_power_draw(Computer& computer);
int computer_power_supply(Computer& computer);
int device_power_draw(Computer& computer, Component type);
int capacity_for_type(Kernel& kernel, Component type);
std::vector<std::string> program_names(const Kernel& kernel);
std::vector<std::string> daemon_names(const Kernel& kernel);
std::vector<std::string> file_names(const Kernel& kernel);
std::vector<std::string> device_names(const Kernel& kernel);
std::vector<std::string> connection_names(const Kernel& kernel);

#pragma raw transformation functions

Result install_kernel(Kernel& kernel, const Computer& computer);
Result install_device(Computer& computer, const Device& device);
Result uninstall_device(Computer& computer, Component type);
Result install_program(Kernel& kernel, const Software& sw);
Result install_daemon(Kernel& kernel, const Software& sw);
Result copy_file(Kernel& kernel, const File& file);
Result connect_to(Kernel& kernel, Kernel& target);
Result uninstall_program(Kernel& kernel, unsigned index);
Result uninstall_daemon(Kernel& kernel, unsigned index);
Result delete_file(Kernel& kernel, unsigned index);
Result disconnect(Kernel& kernel, unsigned index);

#pragma readonly functions

Result info(const Computer& computer);
Result info(const Software& software);
Result info(const File& file);
Result info(const Kernel& kernel);

ResultSet list_programs(const Kernel& kernel);
ResultSet list_daemons(const Kernel& kernel);
ResultSet list_files(const Kernel& kernel);
ResultSet list_connections(const Kernel& kernel);

}	// namespace Auto
