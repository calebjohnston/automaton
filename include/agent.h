//
//  actor.h
//
//  Created by Caleb Johnston on 3/8/2022.
//  Copyright © 2022 Caleb Johnston. All rights reserved.
//

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "component.h"
#include "kernel.h"
#include "program.h"

using KernelRef = std::shared_ptr<class Kernel>;
typedef std::shared_ptr<class Agent> AgentRef;

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

//enum class Intelligence {};
//enum class Drive {};
//enum class Model {};

struct AgentAttribs {
	std::string name = "";
	std::string description = "";
	KernelRef host;
	Status status = Status::None;
	Class type = Class::None;
};

class Agent {
public:
	Agent(const AgentAttribs& attribs) : _name(attribs.name), _descr(attribs.description), _host(attribs.host) {}
	
	std::string name() const { return _name; }
	std::string description() const { return _descr; }
	std::string host() const;
	
protected:
	std::string _name;
	std::string _descr;
	KernelRef _host;
	
};

class Actor : public Agent {
public:
	Actor(const AgentAttribs& attribs) : Agent(attribs), _status(attribs.status), _type(attribs.type) {};
	
protected:
	Status _status;
	Class _type;
	
};

/*
class Automaton : public Actor {
public:
	Automaton(const std::string& name, const std::string& descr, KernelRef host, Status status = Status::None, Class type = Class::None) : Actor(name, descr, host, status, type) {};
	
private:
	Intelligence mind;
	Drive motivation;
	Model model;
};
*/

