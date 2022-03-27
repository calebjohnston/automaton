//
//  network.h
//
//  Created by Caleb Johnston on 3/9/2022.
//  Copyright © 2022 Caleb Johnston. All rights reserved.
//

#pragma once

#include <memory>
#include <string>
#include <vector>
#include <utility>

#include "agent.h"
#include "node.h"

typedef std::shared_ptr<class NetworkNode> NetworkNodeRef;
typedef std::shared_ptr<class Internetwork> InternetworkRef;

enum class NetworkType {
	Hub,
	Bridge,
	Switch,
	Edge,
	Nexus,
	None
};

struct NetworkAttribs {
	std::string name = "";
	std::string description = "";
	NetworkType type = NetworkType::None;
};

class NetworkNode : public GraphNode {
public:
	static NetworkNodeRef create(const NetworkAttribs& attribs, std::vector<GraphNodeRef> children = {});

	std::string description() const { return _descr; }
	NetworkType type() const { return _type; }

private:
	NetworkNode(const NetworkAttribs& attribs)
		: GraphNode({}, attribs.name), _descr(attribs.description), _type(attribs.type) {};
	NetworkNode(std::vector<GraphNodeRef> children, const NetworkAttribs& attribs)
	 	: GraphNode(std::move(children), attribs.name), _descr(attribs.description), _type(attribs.type) {}

	std::string _descr;
	NetworkType _type;

};
/*
class Internetwork {
public:
	Internetwork(const NetworkAttribs& attribs) : _name(attribs.name), _descr(attribs.description) {};

	std::string name() const { return _name; }
	std::string description() const { return _descr; }

private:
	std::string _name;
	std::string _descr;

	std::vector<NetworkRef> _networks;
};
*/
