
#include "agent.h"

using namespace std;

AgentRef Agent::create(KernelRef host, const AgentAttribs& attribs)
{
    return AgentRef(new Agent(host, attribs));
}

Agent::Agent(KernelRef host, const AgentAttribs& attribs)
	: GraphNode({ host }, attribs.name), _descr(attribs.description)
{
}

string Agent::hostname() const
{
	return _children.empty() ? "None" : _children.at(0)->name();
}

ActorRef Actor::create(KernelRef host, const AgentAttribs& attribs)
{
    return ActorRef(new Actor(host, attribs));
}

Actor::Actor(KernelRef host, const AgentAttribs& attribs)
	: Agent(host, attribs), _status(attribs.status), _type(attribs.type)
{
}
