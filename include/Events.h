#pragma once

#include <memory>
#include <string>
#include <functional>

typedef uint64_t EventId;
typedef std::shared_ptr<class Event> EventRef;

/**
 * @brief base type for all events used with EventDispatcher and EventQueue
 *
 * All event handlers receive an shared_ptr<Event> instance for processing.
 * Data that must be transmitted between components should be declared as a
 * property of a type derived from Event.
 *
 * Events sent using EventDispatcher and EventQueue are associated to handlers
 * using a unique event ID. This association happens when a handler (EventListener)
 * is added to an EventDispatcher or EventQueue.
 * @see EventDispatcher::addListener
 * @see EventQueue::addListener
 *
 * The unique event ID is generated at runtime using a string hasher and exposed
 * as a public static constant. This same pattern should be applied to all types
 * derived from Event.
 * @see Event::CHANGE
 * @see Event::UPDATE
 * @see Event::CLOSE
 *
 * Event instances may be stored in several logical structures as it propagates
 * across a program. To manage memory correctly, Event instances should only be
 * used when wrapped by a shared_ptr. To achieve this, all types derived from
 * Event should be constructed using a factory pattern as demonstrated below.
 * @see Event::create()
 *
 * NOTE: _payload member depends upon Variant.h 
 */
class Event {
public:
	static const EventId CHANGE;
	static const EventId UPDATE;
	static const EventId CLOSE;
	static const EventId KEYDOWN;
	static const EventId KEYUP;

public:
	static EventRef create( const EventId id, uint64_t src = 0) { return EventRef( new Event(id, src) ); }
//	static EventRef create( const EventId id , VariantRef payload = nullptr, uint64_t src = 0) { return EventRef( new Event(id, payload, src) ); }

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
//	VariantRef payload() { return _payload; }
	uint64_t src() { return _src; }

protected:
//	explicit Event(const EventId id, VariantRef payload, uint64_t src) : _isHandled(false), _eventId(id), _payload(payload), _src(src) {}
	explicit Event(const EventId id, uint64_t src) : _isHandled(false), _eventId(id), _src(src) {}

private:
	bool	   _isHandled;
	EventId	   _eventId;
//	VariantRef _payload;
	uint64_t   _src;
};
