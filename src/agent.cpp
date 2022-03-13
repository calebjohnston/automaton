
#include "agent.h"

std::string Agent::host() const
{
	return _host ? _host->name() : "None";
}
