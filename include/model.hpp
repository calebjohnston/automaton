//
//  model.hpp
//
//  Created by Caleb Johnston on 7/28/2023.
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
std::string to_str(Packet p) {
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
std::string to_str(Component c) {
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
std::string to_str(Status s) {
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
std::string to_str(Class c) {
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

struct File {
	std::string name;
	std::string description;
	std::string contents;
	int version;
	int size;
};

struct Software {
	std::string name;
	std::string description;
	int size;
	int version;
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

#pragma utility functions

int computer_power_draw(Computer& computer)
{
	return computer.disk.power + computer.memory.power + computer.processor.power + computer.uplink.power;
}

int computer_power_supply(Computer& computer)
{
	return computer.power_unit.power <= 0 ? computer.battery.power : computer.power_unit.power;
}

int device_power_draw(Computer& computer, Component type)
{
	switch (type) {
		case Component::Disk: return computer.disk.power;
		case Component::Memory: return computer.memory.power;
		case Component::Processor: return computer.processor.power;
		case Component::Network: return computer.uplink.power;
		case Component::Battery:
		case Component::Power:
		case Component::None: return 0;
	}
	return 0;
}

int capacity_for_type(Kernel& kernel, Component type)
{
	int total_size = 0;
	int total_capacity = 0;
	switch (type) {
		case Component::Disk:
			total_size = std::accumulate(kernel.files.begin(), kernel.files.end(), 0, [](int acc, const File& file){ return acc += file.size; });
			total_capacity = kernel.computer.disk.capacity;
			
		case Component::Memory:
			total_size = std::accumulate(kernel.daemons.begin(), kernel.daemons.end(), 0, [](int acc, const Software& daemon){ return acc += daemon.size; });
			total_capacity = kernel.computer.memory.capacity;
			
		case Component::Processor:
			total_size = std::accumulate(kernel.programs.begin(), kernel.programs.end(), 0, [](int acc, const Software& prog){ return acc += prog.size; });
			total_capacity = kernel.computer.processor.capacity;
			
		case Component::Network:
			total_size = static_cast<int>(kernel.connections.size());
			total_capacity = kernel.computer.uplink.capacity;
			
		default: return 0;
	}
	return total_capacity - total_size;
}

#pragma transformation functions

Result install_kernel(Kernel& kernel, const Computer& computer)
{
	Result res = { 0, "success" };
	
	// input validation
	int total_file_size = std::accumulate(kernel.files.begin(), kernel.files.end(), 0, [](int acc, const File& file){ return acc += file.size; });
	int disk_requirement = computer.disk.capacity - total_file_size;
	int total_mem_size = std::accumulate(kernel.daemons.begin(), kernel.daemons.end(), 0, [](int acc, const Software& daemon){ return acc += daemon.size; });
	int mem_requirement = computer.memory.capacity - total_mem_size;
	int total_proc_size = std::accumulate(kernel.programs.begin(), kernel.programs.end(), 0, [](int acc, const Software& prog){ return acc += prog.size; });
	int proc_requirement = computer.processor.capacity - total_proc_size;
	int conn_requirement = computer.uplink.capacity - static_cast<int>(kernel.connections.size());
	if (disk_requirement < 0 || mem_requirement < 0 || proc_requirement < 0 || conn_requirement < 0) {
		return { 1, "computer has insufficient capacity to install kernel" };
	}
	
	kernel.computer = computer;
	
	return res;
}

Result install_device(Computer& computer, const Device& device)
{
	Result res = { 0, "success" };
	if (Component::Battery != device.type && Component::Power != device.type) {
		int total_draw = computer_power_draw(computer);
		int device_draw = device_power_draw(computer, device.type);
		int total_supply = computer_power_supply(computer);
		if (total_draw - device_draw + device.power > total_supply) {
			return { 2, "total device power draw exceeds power supply: " };
		}
	}

	switch (device.type) {
		case Component::Disk: computer.disk = device; break;
		case Component::Memory: computer.memory = device; break;
		case Component::Processor: computer.processor = device; break;
		case Component::Network: computer.uplink = device; break;
		case Component::Battery: computer.battery = device; break;
		case Component::Power: computer.power_unit = device; break;
		default: 
			res = { 1, "no matching device type for: " + device.name };
	}

	return res;
}

Result uninstall_device(Computer& computer, Component type)
{
	Result res = { 0, "success" };
	switch (type) {
		case Component::Disk: computer.disk = {}; break;
		case Component::Memory: computer.memory = {}; break;
		case Component::Processor: computer.processor = {}; break;
		case Component::Network: computer.uplink = {}; break;
		case Component::Battery: computer.battery = {}; break;
		case Component::Power: computer.power_unit = {}; break;
		default: 
			res = { 1, "no matching device type" };
	}

	return res;
}

Result install_program(Kernel& kernel, const Software& sw)
{
	Result res = { 0, "success" };

	if (Binary::Program != sw.exec_type) return { 2, "specified software is not a program" };
	
	int capacity = capacity_for_type(kernel, Component::Processor);
	if (capacity > sw.size) {
		kernel.programs.push_back(sw);
	}
	else res = { 1, "program is too large for remaining processor capacity" };

	return res;
}

Result install_daemon(Kernel& kernel, const Software& sw)
{
	Result res = { 0, "success" };
	
	if (Binary::Daemon != sw.exec_type) return { 2, "specified software is not a daemon" };
	
	int capacity = capacity_for_type(kernel, Component::Memory);
	if (capacity > sw.size) {
		kernel.daemons.push_back(sw);
	}
	else res = { 1, "daemon is too large for remaining memory capacity" };
	
	return res;
}

Result copy_file(Kernel& kernel, const File& file)
{
	Result res = { 0, "success" };
	
	int capacity = capacity_for_type(kernel, Component::Disk);
	if (capacity > file.size) {
		kernel.files.push_back(file);
	}
	else res = { 1, "file is too large for remaining disk capacity" };
	
	return res;
}

Result connect_to(Kernel& kernel, Kernel& target)
{
	Result res = { 0, "success" };
	
	int capacity = capacity_for_type(kernel, Component::Network);
	if (capacity > 0) {
		kernel.connections.push_back(target);
	}
	else res = { 1, "maximum number of simultaneous connections reached" };
	
	return res;
}

Result uninstall_program(Kernel& kernel, uint index)
{
	if (index >= kernel.programs.size()) return { 1, "invalid index" };
	
	kernel.programs.erase(kernel.programs.begin() + index);
	
	return { 0, "success" };
}

Result uninstall_daemon(Kernel& kernel, uint index)
{
	if (index >= kernel.daemons.size()) return { 1, "invalid index" };
	
	kernel.daemons.erase(kernel.daemons.begin() + index);
	
	return { 0, "success" };
}

Result delete_file(Kernel& kernel, uint index)
{
	if (index >= kernel.files.size()) return { 1, "invalid index" };
	
	kernel.files.erase(kernel.files.begin() + index);
	
	return { 0, "success" };
}

Result disconnect(Kernel& kernel, uint index)
{
	if (index >= kernel.connections.size()) return { 1, "invalid index" };
	
	kernel.connections.erase(kernel.connections.begin() + index);
	
	return { 0, "success" };
}

#pragma readonly functions

Result info(const Computer& computer)
{
	return { 0, "Computer: #" + computer.serial + " " + computer.manufacturer };
}

Result info(const Software& software)
{
	std::string label;
	if (Binary::Daemon == software.exec_type) label = "Daemon";
	else if (Binary::Program == software.exec_type) label = "Program";
	
	return { 0, label + ": " + software.name + " " + std::to_string(software.size) + " " + std::to_string(software.version) + " " + std::to_string(software.cycles) + " " + software.description };
}

Result info(const File& file)
{
	return { 0, "File: " + file.name + " " + std::to_string(file.size) + " " + std::to_string(file.version) + " " + file.description };
}

Result info(const Kernel& kernel)
{
	return { 0, "Kernel: " + kernel.hostname + " " + std::to_string(kernel.version) + " " + std::to_string(kernel.programs.size()) + " " + std::to_string(kernel.daemons.size()) + " " + std::to_string(kernel.files.size()) + " " + kernel.computer.manufacturer };
}

template<typename T>
ResultSet to_results(const std::vector<T>& list)
{
	ResultSet out;
	for (int i = 0; i < list.size(); i++) {
		Result res = info(list[i]);
		res.status = i;
		out.push_back(res);
	}
	
	return out;
}

ResultSet list_programs(const Kernel& kernel)
{
	return to_results<Software>(kernel.programs);
}

ResultSet list_daemons(const Kernel& kernel)
{
	return to_results<Software>(kernel.daemons);
}

ResultSet list_files(const Kernel& kernel)
{
	return to_results<File>(kernel.files);
}

ResultSet list_connections(const Kernel& kernel)
{
	return to_results<Kernel>(kernel.connections);
}

}	// namespace Auto
