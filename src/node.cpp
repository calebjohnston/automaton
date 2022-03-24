#include <memory>
#include <vector>

#include "node.h"

using namespace std;

int GridNode::add(GridNodeRef child)
{
	if (!child) return -1;

	_children.emplace_back(child);
	return _children.size() - 1;
}

GridNodeRef GridNode::get(size_t index) const
{
	if (index >= _children.size()) return GridNodeRef();

	return _children.at(index);
}

bool GridNode::remove(size_t index)
{
	if (index >= _children.size()) return false;

	_children.erase(_children.begin() + index);
}



int GraphNode::add(GraphNodeRef child)
{
	if (!child) return -1;

	_children.emplace_back(child);
	return _children.size() - 1;
}

GraphNodeRef GraphNode::get(size_t index) const
{
	if (index >= _children.size()) return GraphNodeRef();

	return _children.at(index);
}

bool GraphNode::remove(size_t index)
{
	if (index >= _children.size()) return false;

	_children.erase(_children.begin() + index);
}
