#pragma once

#include <deque>
#include <functional>

#include "Events.h"

#include "eventpp/callbacklist.h"
#include "eventpp/eventqueue.h"

using EventId			= uint64_t;
using Callback			= void(const EventRef);
using EventListener		= std::function<Callback>;
using EventQueueMap		= eventpp::EventQueue<EventId, Callback>;
using QueueHandle		= EventQueueMap::Handle;

/**
 * @brief an asynchronous, thread-safe variant of the event dispatcher
 *
 * The EventQueue stores a mapping between unique event IDs and handlers,
 * called EventListeners. When the update() method is invoked, all EventListeners
 * will be immediately triggered that were associated with the Event's ID. Each
 * event handler will receive a shared pointer to an Event instance.
 * EventListeners can be added any time. Events may be queued at any time.
 * But the upate() method should only be invoked as part of an update cycle.
 * @see Event
 * @see EventQueue::enqueue()
 * @see EventQueue::update()
 *
 * Event instances may be stored in several logical structures as it propagates
 * through event handlers. To ensure that the memory is managed correctly,
 * Events are always wrapped in shared_ptrs.
 * @see Event::create()
 *
 * The method signatures of EventQueue match EventDispatcher, the synchronous
 * variant of EventQueue.
 * @see EventDispatcher
 *
 */
class EventQueue {
	using EventDeque	= std::deque<EventRef>;

public:
	explicit EventQueue() = default;
	virtual ~EventQueue() = default;

	virtual QueueHandle addListener(const EventListener& listener, const EventId& type);
	virtual bool removeListener(const QueueHandle& handle, const EventId& type);
	virtual bool hasListener(const EventId& type);

	virtual bool enqueue(const EventRef event);
	// virtual bool cancel( const EventId& type );

	virtual void update();

private:
	EventQueueMap _eventListeners;
	EventDeque _queue;
};

/**
 * @brief a singleton interface for an asynchronous EventQueue instance
 *
 * The Messenger is a thin wrapper around the EventQueue that may be used
 * as a central event bus for all components to integrate with.
 * @see EventQueue
 *
 * For synchronous events, use an EventDispatcher instance.
 * @see EventDispatcher
 */
class Messenger {
public:
	/**
	 * Removing all copy constructors as the Messenger must be instantiated
	 * using the Controller::instance() accessor method
	 */
	Messenger(const Messenger& controller) noexcept = delete;
	Messenger(Messenger&& controller) noexcept = delete;
	Messenger& operator=(const Messenger& controller) noexcept = delete;
	Messenger& operator=(Messenger&& controller) noexcept = delete;
	
	QueueHandle addListener(const EventListener& listener, const EventId& type);
	bool removeListener(const QueueHandle& handle, const EventId& type);
	bool hasListener(const EventId& type);
	bool enqueue(const EventRef event);
	void update();
	
	//! Singleton accessor method. Returns instance
	static Messenger* instance() { return s_initialized ? &s_instance : nullptr; }
	
private:
	Messenger();
	~Messenger() = default;
	
	static Messenger s_instance;
	static bool s_initialized;
	
	EventQueue _queue;
};
