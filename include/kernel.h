//
//  kernel.h
//
//  Created by Caleb Johnston on 3/8/2022.
//  Copyright © 2022 Caleb Johnston. All rights reserved.
//

#pragma once

#include <functional>
#include <iterator>
#include <memory>
#include <string>
#include <vector>

#include "data.h"
#include "network.h"
#include "program.h"
#include "table.hpp"

using AgentRef = std::shared_ptr<class Agent>;
using ComputerRef = std::shared_ptr<class Computer>;
using NetworkRef = std::shared_ptr<class Network>;
typedef std::shared_ptr<class Kernel> KernelRef;

class Subsystem {
//public:
//	typedef std::function<void(DataRef)> Predicate;
	
public:
	Subsystem(size_t limit = 0) : _capacity(limit), _consumed(0) {}
	
	bool load(DataRef record)
	{
		if (record && _capacity - _consumed < record->size) return false;
		
		_data.insert(record);
		_consumed += record->size;
		return true;
	}
	
	bool unload(DataRef record)
	{
		if (!record) return false;
		int size = record->size;
		
		bool removed = _data.remove(record);
		_consumed -= removed ? size : 0;
		return removed;
	}
	
	bool loaded()
	{
		std::function<bool(DataRef)> filter = [](DataRef record) {
			return record && record->size > 2;
		};
//		int idx = 0;
//		std::function<std::tuple<std::string,std::string>(DataRef)> trans = [](DataRef record) {
//			return std::make_tuple(record->name, record->description);
//		};
//		
//		auto results = _data.filter<std::tuple<std::string,std::string>>(_data.query(filter), trans);
	}
	
//	template<typename T, typename Predicate>
//	auto loaded(Predicate filter) {
//		std::vector<T> results;
//		std::copy_if(std::begin(_data), std::end(_data), std::back_inserter(results), filter);
//		return results;
//	}
	
private:
	size_t _capacity;
	size_t _consumed;
	Table<DataRef> _data;
};

class Kernel {
public:
	Kernel(const std::string& name, const std::string& descr, ComputerRef computer);
	
	// render functions
	std::string name() const { return _name; }
	std::string description() const { return _description; }
	std::string host() const;
	
	bool install(SoftwareRef software) { return _registry.load(software); }
	bool uninstall(SoftwareRef software) { return _registry.unload(software); }
	std::vector<SoftwareRef> installedSoftware() const;
	
	bool load(DataRef data) { return _registry.load(data); }
	bool unload(DataRef data) { return _registry.unload(data); }
	std::vector<DataRef> loadedData() const;
	
	bool trust(AgentRef agent) { return _group.ensure(agent); }
	bool distrust(AgentRef agent) { return _group.remove(agent); }
	std::vector<AgentRef> trustedAgents() const;
	
	bool connect(NetworkRef network) { _network = network; return _network.get(); }
	bool disconnect() { _network.reset(); return true; }
	std::vector<AgentRef> networkedAgents() const;
	
protected:
	std::string _name;
	std::string _description;
	
	ComputerRef _machine;
	NetworkRef _network;
	
	Table<AgentRef> _group;
	
	Subsystem _filesystem;
	Subsystem _registry;
};
