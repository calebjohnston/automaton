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

#include "agent.h"
#include "data.h"
#include "controller.h"
#include "network.h"
#include "node.h"
#include "program.h"

using AgentRef = std::shared_ptr<class Agent>;
using ComputerRef = std::shared_ptr<class Computer>;
typedef std::shared_ptr<class Kernel> KernelRef;

class Kernel : public GraphNode {
public:
	static KernelRef create(ComputerRef computer, std::string name, std::string descr);

	std::string description() const { return _description; }
	std::string host() const;

	virtual void connect(GridNodeRef grid) override;

	virtual int add(GraphNodeRef child) override;

	virtual bool remove(size_t index) override;

	// acts as command processor but delegates command execution to controllers

	// bool load(ProgramRef program) { return _children[ProcCtrlIdx]->add(program) >= 0; }
	// bool unload(size_t index) { return _children[ProcCtrlIdx]->remove(index); }
	// std::vector<ProgramRef> programs() const;

	bool install(DaemonRef daemon) { return _children[MemCtrlIdx]->add(daemon) >= 0; }
	bool uninstall(size_t index) { return _children[MemCtrlIdx]->remove(index); }
	std::vector<DaemonRef> daemons() const;

	bool copy(DataRef data) { return _children[DiskCtrlIdx]->add(data) >= 0; }
	bool erase(size_t index) { return _children[DiskCtrlIdx]->remove(index); }
	std::vector<DataRef> filesystem() const;

	bool trust(AgentRef agent); //!< impl in cpp to resolve compiler error with incomplete type AgentRef from "using" declaration above
	bool distrust(size_t index) { return _children[AgentsCtrlIdx]->remove(index); }
	std::vector<AgentRef> group() const;

	bool connect(KernelRef host) {
		if (connected(host)) return false; // prevents infinite loop

		bool success = _children[ConnCtrlIdx]->add(host) >= 0 && host->connect(shared_from_this());
	}
	bool disconnect(size_t index) {
		// first disconnect the remote end ...
		return _children[ConnCtrlIdx]->remove(index);
	}

	std::vector<KernelRef> connections() const;

	// processCommand() -> switch/match against handler functions
	class Command {
		AgentRef sender;
		CommandType type;
		Payload data;
	};
	void process(Command); // -> filter by trusted agent, route to Controller based upon type, and then invoke matching create/read/update/delete function with the payload as parameter
	void receive(Command); // -> copy Command(?) from open connection and then invoke process?
	void dispatch(Command,KernelRef); // -> send through connection

	void process(Packet); // -> for Daemons only
	void receive(Packet); // -> copy to daemon?
	void dispatch(Packet,KernelRef); // -> for Programs only

protected:
	Kernel(ComputerRef computer, const std::string& name, const std::string& descr);

	std::string _description;

	const static size_t DiskCtrlIdx = 0;
	const static size_t MemCtrlIdx = 1;
	const static size_t ProcCtrlIdx = 2;
	const static size_t ConnCtrlIdx = 3;
	const static size_t AgentsCtrlIdx = 4;
	const static size_t MaxChildren = 5;

	// static bool onInstallProgram(KernelRef kernel, ProgramRef prog, AgentRef agent)
	// {
	// 	ProgramRef prog = cmd.program;
	// 	AgentRef agent = cmd.agent;
	//
	// 	if (!isTrusted(agent)) return false;
	//
	// 	kernel->install(prog);
	// 	return true;
	// }
};
