//
//  network.hpp
//
//  Created by Caleb Johnston on 3/9/2022.
//  Copyright © 2022 Caleb Johnston. All rights reserved.
//

#pragma once

#include <memory>
#include <string>
#include <vector>
#include <utility>

#include "actor.hpp"

using HostRef = std::shared_ptr<Host>;
using NetworkRef = std::shared_ptr<class Network>;
using InternetworkRef = std::shared_ptr<class Internetwork>;
using InternetworkLink = std::pair<InternetworkRef,HostRef>;

enum class NetworkType {
	Hub,
	Bridge,
	Switch,
	EdgeNode,
	Supernode,
	Nexus,
	None
};

class Network {
public:
	std::string name;
	std::string description;
	NetworkType type;
	
	InternetworkLink inetLink;
	std::vector<HostRef> nodes;
	
	bool operator ==(const Network& rhs) const {
		return  name == rhs.name && description == rhs.description;
	}
	
	bool operator !=(const Network& rhs) const { return !(*this == rhs); }
	bool operator <(const Network& rhs) const { return name < rhs.name; }
	bool operator >(const Network& rhs) const { return name > rhs.name; }
	
	operator bool() const {
		return !name.empty() && !description.empty() && type != NetworkType::None;
	}
	
protected:
	Network() : name(""), description(""), type(NetworkType::None) {};
	
};


class Internetwork {
public:
	std::string name;
	std::string description;
	
	std::vector<InternetworkLink> internets;
	std::vector<NetworkRef> networks;
};
