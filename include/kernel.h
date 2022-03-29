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
#include "node.h"
#include "program.h"
#include "slot.hpp"

using AgentRef = std::shared_ptr<class Agent>;
using ComputerRef = std::shared_ptr<class Computer>;
// using NetworkRef = std::shared_ptr<class Network>;
typedef std::shared_ptr<class Controller> ControllerRef;
typedef std::shared_ptr<class Kernel> KernelRef;

class Controller : public GraphNode {
public:
	static ControllerRef create(ComponentRef component, std::string name = "");

	virtual void connect(GridNodeRef grid) override;

	virtual int add(GraphNodeRef child) override;

	virtual bool remove(size_t index) override;

protected:
	Controller(ComponentRef component, const std::string& name);

	int _capacity;
};

class Kernel : public GraphNode {
public:
	static KernelRef create(ComputerRef computer, std::string name, std::string descr);

	std::string description() const { return _description; }
	std::string host() const;

	virtual void connect(GridNodeRef grid) override;

	virtual int add(GraphNodeRef child) override;

	virtual bool remove(size_t index) override;
/*
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
*/
	// bool connect(NetworkRef network) { _network = network; return _network.get(); }
	// bool disconnect() { _network.reset(); return true; }
	// std::vector<AgentRef> networkedAgents() const;

protected:
	Kernel(ComputerRef computer, const std::string& name, const std::string& descr);

	std::string _description;

	// Table<AgentRef> _group;
	Slot<Controller,GraphNode,0> _diskController;

	const static size_t DiskCtrlIdx = 0;
	const static size_t MemCtrlIdx = 1;
	const static size_t ProcCtrlIdx = 2;
	const static size_t ConnCtrlIdx = 3;
	const static size_t AgentsCtrlIdx = 4;
	const static size_t MaxChildren = 5;
};
