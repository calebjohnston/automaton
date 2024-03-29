#pragma once

#include <memory>
#include <string>
#include <vector>

typedef std::shared_ptr<class GridNode> GridNodeRef;
typedef std::shared_ptr<class GraphNode> GraphNodeRef;

class GridNode {
public:
	static GridNodeRef create(int power = 0, std::vector<GridNodeRef> children = {}, std::string name = "");

	virtual ~GridNode() = default;

	int power() const { return _power; }

	std::string name() const { return _name; }

	const std::vector<GridNodeRef>& children() const { return _children; }

	virtual int add(GridNodeRef child);

	virtual GridNodeRef get(size_t index) const;

	virtual bool remove(size_t index);

	size_t size() const { return _children.size(); }

protected:
	GridNode(int power, std::vector<GridNodeRef> children, std::string name)
	 	: _power(power), _children(std::move(children)), _name(std::move(name)) {}

	int _power;
	std::string _name;
	std::vector<GridNodeRef> _children;
};

class GraphNode {
public:
	static GraphNodeRef create(std::vector<GraphNodeRef> children = {}, std::string name = "");

	virtual ~GraphNode() = default;

	std::string name() const { return _name; }

	const std::vector<GraphNodeRef>& children() const { return _children; }

	virtual void connect(GridNodeRef grid) { _gridNode = grid; }

	virtual int add(GraphNodeRef child);

	virtual GraphNodeRef get(size_t index) const;

	virtual bool remove(size_t index);

	size_t size() const { return _children.size(); }

protected:
	GraphNode(std::vector<GraphNodeRef> children, std::string name)
	 	: _children(std::move(children)), _name(std::move(name)) {}

	std::string _name;
	std::vector<GraphNodeRef> _children;
	GridNodeRef _gridNode;
};
