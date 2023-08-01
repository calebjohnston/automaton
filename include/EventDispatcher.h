#pragma once

#include <functional>

#include "Events.h"

#include "eventpp/callbacklist.h"
#include "eventpp/eventdispatcher.h"

using EventId			= uint64_t;
using Callback			= void(const EventRef);
using EventListener		= std::function<Callback>;
using EventDispatchMap	= eventpp::EventDispatcher<EventId, Callback>;
using DispatchHandle	= EventDispatchMap::Handle;

/**
 * @brief synchronous, thread-safe event dispatcher
 *
 * The EventDispatcher stores a mapping between unique event IDs and handlers,
 * called EventListeners. When the dispatch() method is invoked, all EventListeners
 * will be immediately triggered that were associated with the Event's ID. Each
 * event handler will receive a shared pointer to an Event instance.
 * @see Event
 * @see EventDispatcher::dispatch()
 *
 * Event instances may be stored in several logical structures as it propagates
 * through event handlers. To ensure that the memory is managed correctly,
 * Events are always wrapped in shared_ptrs.
 * @see Event::create()
 *
 * The method signatures of EventDispatcher match EventQueue, the asynchronous
 * variant of EventDispatcher.
 * @see EventQueue
 *
 */
class EventDispatcher {
public:
	explicit EventDispatcher() = default;
	virtual ~EventDispatcher() = default;

	virtual DispatchHandle addListener(const EventListener& listener, const EventId& type);
	virtual bool removeListener(const DispatchHandle& handle, const EventId& type);
	virtual bool hasListener(const EventId& type);
	virtual bool dispatch(const EventRef event);

private:
	EventDispatchMap _eventListeners;
};
