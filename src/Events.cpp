#include "Events.h"

static uint64_t GetHash(const std::string& name)
{
	static std::hash<std::string> hasher;
	return hasher( name );
}

const EventId Event::CHANGE  = GetHash( "EventUpdated" );
const EventId Event::UPDATE  = GetHash( "EventChanged" );
const EventId Event::CLOSE   = GetHash( "EventClosed" );
const EventId Event::KEYDOWN = GetHash( "EventKeyDown" );
const EventId Event::KEYUP   = GetHash( "EventKeyUp" );
