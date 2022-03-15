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

class Kernel {
public:
	Kernel(const std::string& name, const std::string& descr, ComputerRef computer);
	
	// render functions
	std::string name() const { return _name; }
	std::string description() const { return _description; }
	std::string host() const;
	
	bool install(ProgramRef program) { return _programRegistry.load(program); }
	bool uninstall(ProgramRef program) { return _programRegistry.unload(program); }
	std::vector<ProgramRef> installedPrograms() const;
	
	bool install(DaemonRef daemon) { return _daemonRegistry.load(daemon); }
	bool uninstall(DaemonRef daemon) { return _daemonRegistry.unload(daemon); }
	std::vector<DaemonRef> installedDaemons() const;
	
	bool load(DataRef data) { return _filesystem.load(data); }
	bool unload(DataRef data) { return _filesystem.unload(data); }
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
	
	DataController _filesystem;
	DataController _programRegistry;
	DataController _daemonRegistry;
};
