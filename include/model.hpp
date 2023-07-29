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

namespace Auto {

enum class Packet {
	Ping,
	Inference,
	Reflection,
	Symplex,
	Autoregression,
	None
};
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
};

struct Agent {
	std::string name;
	std::string description;
	Status status;
	Class type;
	int version;
};

struct Result {
	int status;
	std::string message;
};

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

int kernel_capacity_for_type(Kernel& kernel, Component type)
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

Result install_program(Kernel& kernel, const Software& sw)
{
	Result res = { 0, "success" };

	if (Binary::Program != sw.exec_type) return { 2, "specified software is not a program" };
	
	int capacity = kernel_capacity_for_type(kernel, Component::Processor);
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
	
	int capacity = kernel_capacity_for_type(kernel, Component::Memory);
	if (capacity > sw.size) {
		kernel.daemons.push_back(sw);
	}
	else res = { 1, "daemon is too large for remaining memory capacity" };
	
	return res;
}

Result copy_file(Kernel& kernel, const File& file)
{
	Result res = { 0, "success" };
	
	int capacity = kernel_capacity_for_type(kernel, Component::Disk);
	if (capacity > file.size) {
		kernel.files.push_back(file);
	}
	else res = { 1, "file is too large for remaining disk capacity" };
	
	return res;
}

Result connect_to(Kernel& kernel, Kernel& target)
{
	Result res = { 0, "success" };
	
	int capacity = kernel_capacity_for_type(kernel, Component::Network);
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

}	// namespace Auto
