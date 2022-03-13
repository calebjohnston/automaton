
#include "kernel.h"
#include "computer.h"

Kernel::Kernel(const std::string& name, const std::string& descr, ComputerRef computer)
: _name(name), _description(descr), _machine(computer)
{
	if (_machine) {
		_registry = Subsystem(_machine->memory.capacity);
		_filesystem = Subsystem(_machine->storage.capacity);
	}
}

std::string Kernel::host() const
{
	return _machine ? _machine->name : "None";
}

std::vector<SoftwareRef> Kernel::installedSoftware() const
{
//	_registry
}

std::vector<DataRef> Kernel::loadedData() const
{
}

std::vector<AgentRef> Kernel::trustedAgents() const
{
//	query(, <#RowPredicate filter#>)
//	std::vector<AgentRef> results = _group.query([](AgentRef agent) { return agent->name() != ""; });
//	Table<AgentRef>::fields<std::tuple<std::string>>(results, [](AgentRef agent) { return std::make_tuple(agent->description()); });
}

std::vector<AgentRef> Kernel::networkedAgents() const
{
}
	
//	ComputerRef _machine;
//	NetworkRef _network;
//
//	Table<AgentRef> _group;
//
//	Subsystem _filesystem;
//	Subsystem _registry;
