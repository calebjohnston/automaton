#include "EventQueue.h"

using namespace std;

QueueHandle EventQueue::addListener(const EventListener& listener, const EventId& type)
{
	return _eventListeners.appendListener(type, listener);
}

bool EventQueue::removeListener(const QueueHandle& handle, const EventId& type)
{
	return _eventListeners.removeListener(type, handle);
}

bool EventQueue::hasListener(const EventId& type)
{
	return _eventListeners.hasAnyListener(type);
}

bool EventQueue::enqueue(const EventRef event)
{
	if (!event) return false;

	_eventListeners.enqueue(event->getEventId(), event);

	return true;
}

void EventQueue::update()
{
	_eventListeners.process();
}

Messenger Messenger::s_instance;
bool Messenger::s_initialized = false;

Messenger::Messenger()
{
	s_initialized = true;
}

QueueHandle Messenger::addListener(const EventListener& listener, const EventId& type)
{
	if (!s_initialized) return QueueHandle();
	
	return _queue.addListener(listener, type);
}

bool Messenger::removeListener(const QueueHandle& handle, const EventId& type)
{
	if (!s_initialized) return false;
	
	return _queue.removeListener(handle, type);
}

bool Messenger::hasListener(const EventId& type)
{
	if (!s_initialized) return false;
	
	return _queue.hasListener(type);
}

bool Messenger::enqueue(const EventRef event)
{
	if (!s_initialized) return false;
	
	return _queue.enqueue(event);
}

void Messenger::update()
{
	if (s_initialized) _queue.update();
}
