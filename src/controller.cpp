
#include <algorithm>
#include <string>
#include <vector>

#include "computer.h"
#include "controller.h"
#include "kernel.h"

using namespace std;

template<typename T, typename U>
auto convert(const std::vector<std::shared_ptr<U>>& input)
{
	struct DynamicCaster {
		std::shared_ptr<T> operator()(std::shared_ptr<U> value) const {
			return std::dynamic_pointer_cast<T>(value);
		}
	};

    std::vector<std::shared_ptr<T>> result;
    std::transform(input.begin(), input.end(), result.begin(), DynamicCaster());

	return result;
}


ControllerRef Controller::create(ComponentRef component, std::string name)
{
	return ControllerRef(new Controller(component, std::move(name)));
}

Controller::Controller(ComponentRef component, const std::string& name)
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


ProgramControllerRef ProgramController::create(ComponentRef component, std::string name = "")
{
	return ProgramControllerRef(new ProgramController(component, std::move(name)));
}

ProgramController::ProgramController(ComponentRef component, const std::string& name)
	: Controller(component, name)
{
}

std::vector<ProgramRef> ProgramController::loaded() const
{
	return convert<Program, GraphNode>(_children[ProcCtrlIdx]->children());
}

bool ProgramController::loaded(ProgramRef program) const
{
	return std::find(begin(_children), end(_children), program) != std::end(_children)
}
