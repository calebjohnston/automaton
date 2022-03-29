#include <type_traits>

#include "kernel.h"
#include "computer.h"

using namespace std;

ControllerRef Controller::create(ComponentRef component, string name)
{
	return ControllerRef(new Controller(component, std::move(name)));
}

Controller::Controller(ComponentRef component, const string& name)
	: GraphNode({}, name)
{
	// static_assert(computer)
	connect(component);
}

void Controller::connect(GridNodeRef grid)
{
	ComponentRef component = dynamic_pointer_cast<Component>(grid);
	_gridNode = component;
	_capacity = _gridNode->size();
	_children.clear();
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

KernelRef Kernel::create(ComputerRef computer, string name, string descr)
{
	return KernelRef(new Kernel(std::move(computer), std::move(name), std::move(descr)));
}

Kernel::Kernel(ComputerRef computer, const string& name, const string& descr)
: GraphNode({}, name), _diskController(_children), _description(descr)
{
	connect(computer);

	_diskController->size();
}

std::string Kernel::host() const
{
	return _gridNode ? _gridNode->name() : "None";
}

void Kernel::connect(GridNodeRef grid)
{
	ComputerRef computer = dynamic_pointer_cast<Computer>(grid);
	_gridNode = computer;

	_children[Kernel::DiskCtrlIdx] = Controller::create(computer->disk());
	_children[Kernel::MemCtrlIdx] = Controller::create(computer->memory());
	_children[Kernel::ProcCtrlIdx] = Controller::create(computer->processor());
	_children[Kernel::ConnCtrlIdx] = Controller::create(computer->uplink());
	_children[Kernel::AgentsCtrlIdx] = GraphNodeRef(nullptr);
}

int Kernel::add(GraphNodeRef child)
{
	// NOT IMPLEMENTED
	ControllerRef controller = dynamic_pointer_cast<Controller>(child);
	_diskController = controller;

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
