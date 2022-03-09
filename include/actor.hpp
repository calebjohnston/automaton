//
//  actor.hpp
//
//  Created by Caleb Johnston on 3/8/2022.
//  Copyright © 2022 Caleb Johnston. All rights reserved.
//

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "component.hpp"
#include "program.hpp"

enum class Status {
	Active,
	Inactive,
	Busy,
	None
};
enum class Class {
	Electronics,
	Monitor,
	Sentry,
	Worm,
	Virus,
	Simulation,
	Agent,
	Automaton,
	None
};

using HostRef = std::shared_ptr<class Host>;
using NetworkRef = std::shared_ptr<class Network>;

class Host {
public:
	std::string name;
	std::string description;
	Computer machine;
	Kernel system;
	
	std::vector<HostRef> peers;
	NetworkRef network;
	
	bool operator ==(const Host& rhs) const {
		return  name == rhs.name && description == rhs.description;
	}
	
	bool operator !=(const Host& rhs) const { return !(*this == rhs); }
	bool operator <(const Host& rhs) const { return name < rhs.name; }
	bool operator >(const Host& rhs) const { return name > rhs.name; }
	
	operator bool() const { return !name.empty(); }
	
protected:
	Host() : name(""), description("") {};
};

class Actor : public Host {
public:
	Status status;
	Class type;
	
protected:
	Actor() : Host(), status(Status::None), type(Class::None) {};
};

