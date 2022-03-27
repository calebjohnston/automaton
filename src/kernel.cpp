
#include "kernel.h"
#include "computer.h"

using namespace std;

ControllerRef Controller::create(ComponentRef component, std::string name)
{
	return ControllerRef(new Controller(component, std::move(name)));
}

Controller::Controller(ComponentRef component, const std::string& name)
	: GraphNode({}, name)
{
	// static_assert(computer)
	_gridNode = component;

	_capacity = _gridNode->size();
}

int Controller::add(GraphNodeRef child)
{
	DataRef record = dynamic_pointer_cast<Data>(child);
	if (!record) return -1;

	ComponentRef component = dynamic_pointer_cast<Component>(_gridNode);
	if (!component) return -1;

	if (_capacity < record->size()) return -1;

	int index = GraphNode::add(child);
	if (index >= 0)
		_capacity -= record->size();

	return index;
}

bool Controller::remove(size_t index)
{
	if (index >= _children.size()) return false;

	int size = _children.at(index)->size();
	bool result = GraphNode::remove(index);
	if (result) _capacity += size;

	return result;
}

KernelRef Kernel::create(ComputerRef computer, std::string name, std::string descr)
{
	return KernelRef(new Kernel(std::move(computer), std::move(name), std::move(descr)));
}

Kernel::Kernel(ComputerRef computer, const std::string& name, const std::string& descr)
: GraphNode({}, name), _description(descr)
{
	// static_assert(computer)
	_gridNode = computer;

	_children[_diskCtrlIdx] = Controller::create(computer->disk());
	_children[_memCtrlIdx] = Controller::create(computer->memory());
	_children[_procCtrlIdx] = Controller::create(computer->processor());
	_children[_connCtrlIdx] = Controller::create(computer->uplink());
	_children[_agentsCtrlIdx] = GraphNodeRef(nullptr);
}

std::string Kernel::host() const
{
	return _gridNode ? _gridNode->name() : "None";
}

int Kernel::add(GraphNodeRef child)
{
	// NOT IMPLEMENTED

	return -1;
}

bool Kernel::remove(size_t index)
{
	// NOT IMPLEMENTED

	return false;
}


/*
std::vector<ProgramRef> Kernel::installedPrograms() const
{
//	_registry
}

std::vector<DaemonRef> Kernel::installedDaemons() const
{
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
*/
// std::vector<AgentRef> Kernel::networkedAgents() const
// {
// }

//	ComputerRef _machine;
//	NetworkRef _network;
//
//	Table<AgentRef> _group;
//
//	Subsystem _filesystem;
//	Subsystem _registry;
