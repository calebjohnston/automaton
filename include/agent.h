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
#include "node.h"

using KernelRef = std::shared_ptr<class Kernel>;
typedef std::shared_ptr<class Agent> AgentRef;
typedef std::shared_ptr<class Actor> ActorRef;

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
	Status status = Status::None;
	Class type = Class::None;
};

class Agent : public GraphNode {
public:
	static AgentRef create(KernelRef host, const AgentAttribs& attribs);

	std::string description() const { return _descr; }
	std::string hostname() const;

protected:
	Agent(KernelRef host, const AgentAttribs& attribs);

	std::string _descr;

};

class Actor : public Agent {
public:
	static ActorRef create(KernelRef host, const AgentAttribs& attribs);

protected:
	Actor(KernelRef host, const AgentAttribs& attribs);

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
