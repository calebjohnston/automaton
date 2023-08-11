#pragma once

#include <memory>
#include <string>
#include <functional>

#include "eventpp/callbacklist.h"
#include "eventpp/eventdispatcher.h"

namespace Auto {

typedef uint64_t EventId;
typedef std::shared_ptr<class Event> EventRef;

using Callback			= void(const EventRef);
using EventListener		= std::function<Callback>;
using EventDispatchMap	= eventpp::EventDispatcher<EventId, Callback>;
using DispatchHandle	= EventDispatchMap::Handle;

class Event {
public:
	static const EventId CHANGE;
	static const EventId UPDATE;
	static const EventId CLOSE;
	static const EventId KEYDOWN;
	static const EventId KEYUP;
	
public:
	static EventRef create( const EventId id, uint64_t src = 0) { return EventRef( new Event(id, src) ); }
	
	virtual ~Event() = default;
	
	virtual EventId getEventId() const { return _eventId; }
	
	static EventId getEvent(const EventRef event) {
		if (event)
			return event->_eventId;
		else
			return 0;
	}
	
	bool isHandled() const { return _isHandled; }
	void setHandled( bool handled = true ) { _isHandled = handled; }
	uint64_t src() { return _src; }
	
protected:
	explicit Event(const EventId id, uint64_t src) : _isHandled(false), _eventId(id), _src(src) {}
	
private:
	bool	   _isHandled;
	EventId	   _eventId;
	uint64_t   _src;
};

}
