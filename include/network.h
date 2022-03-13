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
#include "kernel.h"

using KernelRef = std::shared_ptr<class Kernel>;
typedef std::shared_ptr<class Network> NetworkRef;
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

class Network {
public:
	Network(const NetworkAttribs& attribs) : _name(attribs.name), _descr(attribs.description), _type(attribs.type) {};
	
	std::string name() const { return _name; }
	std::string description() const { return _descr; }
	NetworkType type() const { return _type; }
	
private:
	std::string _name;
	std::string _descr;
	NetworkType _type;
	
	InternetworkRef _internet;
	std::vector<KernelRef> _nodes;
	
};

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
