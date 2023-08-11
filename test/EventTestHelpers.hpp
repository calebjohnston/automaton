#pragma once

#include <string>
#include <functional>

#include "gtest/gtest.h"

#include "Events.h"

using namespace std::placeholders;

typedef std::shared_ptr<class CustomEvent> CustomEventRef;

class CustomEvent : public Event {
public:
	static const EventId CUSTOM;
	
public:
	static CustomEventRef create( const EventId id, const std::string& payload ) {
		return CustomEventRef( new CustomEvent(id, payload) );
	}
	
	virtual ~CustomEvent() {};
	
	const std::string& getPayload() const { return _payload; }
	
protected:
	explicit CustomEvent( const EventId id, const std::string& payload )
		: Event(id, 0), _payload(payload) {}
	
private:
	std::string _payload;
};

class CustomReceiver {
public:
	std::string param1;
	void customHandler(const EventRef event) {
		auto custom_event = std::dynamic_pointer_cast<CustomEvent>(event);
		if (custom_event) param1 = custom_event->getPayload();
	}
};

class ExampleListener {
public:
	int param1;
	ExampleListener() : param1(0) {}
	
	void handler(const EventRef event) {
		param1 = 42;
	}
};
