#include "EventDispatcher.h"

using namespace std;

DispatchHandle EventDispatcher::addListener(const EventListener& listener, const EventId& type)
{
	return _eventListeners.appendListener(type, listener);
}

bool EventDispatcher::removeListener(const DispatchHandle& handle, const EventId& type)
{
	return _eventListeners.removeListener(type, handle);
}

bool EventDispatcher::hasListener(const EventId& type)
{
	return _eventListeners.hasAnyListener(type);
}

bool EventDispatcher::dispatch(const EventRef event)
{
	if (!event) return false;

	_eventListeners.dispatch(event->getEventId(), event);

	return hasListener(event->getEventId());
}
