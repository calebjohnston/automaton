#include <array>
#include <functional>
#include <string>

#include "gtest/gtest.h"

#include "Events.h"
#include "EventDispatcher.h"
#include "EventTestHelpers.hpp"

using namespace std::placeholders;

std::hash<std::string> str_hash;
const EventId CustomEvent::CUSTOM = str_hash( "ReceiverCustomEvent" );

class Consumer {
public:
	int received_events = 0;
	void increment(const EventRef event) {
		received_events++;
	}
};

class Reducer {
public:
	static int all_events;
	void increment(const EventRef event) {
		all_events++;
	}
};

int Reducer::all_events = 0;

TEST(EventDispatcher, Destruction)
{
	EventDispatcher* dispatcher = new EventDispatcher();
	ExampleListener receiver1;
	
	dispatcher->addListener(std::bind(&ExampleListener::handler, &receiver1, _1), Event::UPDATE);
	dispatcher->dispatch(Event::create(Event::UPDATE));
	
	EXPECT_NO_THROW(delete dispatcher);
	
	EXPECT_EQ(receiver1.param1, 42);
}

TEST(EventDispatcher, AddListeners)
{
	EventDispatcher dispatcher;
	ExampleListener receiver1;
	CustomReceiver receiver2;
	int context = 0;
	
	dispatcher.addListener(std::bind(&ExampleListener::handler, &receiver1, _1), Event::UPDATE);
	dispatcher.addListener(std::bind(&CustomReceiver::customHandler, &receiver2, _1), Event::UPDATE);
	dispatcher.addListener([&](const EventRef event) { context = 42; }, Event::UPDATE);
	
	EXPECT_NO_THROW(dispatcher.dispatch(Event::create(Event::UPDATE))) << "dispatch method should not throw exception";
	
	EXPECT_EQ(receiver1.param1, 42) << "Public class method event handler should be triggered";
	EXPECT_EQ(receiver2.param1, "") << "Event handler method that applies type downcast should safely act as a no-op";
	EXPECT_EQ(context, 42) << "Lambda event handlers should work as normal";
}

TEST(EventDispatcher, HasListeners)
{
	EventDispatcher dispatcher;
	ExampleListener receiver1;
	CustomReceiver receiver2;
	
	EXPECT_FALSE(dispatcher.hasListener(Event::UPDATE)) << "EventDispatcher::hasListener returns false without any matching listeners";
	dispatcher.addListener(std::bind(&ExampleListener::handler, &receiver1, _1), Event::UPDATE);
	EXPECT_TRUE(dispatcher.hasListener(Event::UPDATE)) << "EventDispatcher::hasListener returns true with matching listener";
	
	EXPECT_FALSE(dispatcher.hasListener(Event::CHANGE)) << "EventDispatcher::hasListener respects input event type";
	dispatcher.addListener(std::bind(&CustomReceiver::customHandler, &receiver2, _1), Event::CHANGE);
	EXPECT_TRUE(dispatcher.hasListener(Event::CHANGE)) << "EventDispatcher::hasListener respects input event type";
}

TEST(EventDispatcher, RemoveListener)
{
	EventDispatcher dispatcher;
	ExampleListener receiver1;
	CustomReceiver receiver2;
	
	auto receiver1_handle = dispatcher.addListener(std::bind(&ExampleListener::handler, &receiver1, _1), Event::UPDATE);
	EXPECT_TRUE(dispatcher.removeListener(receiver1_handle, Event::UPDATE)) << "EventDispatcher::removeListener returns true when matching listener was removed";
	EXPECT_FALSE(dispatcher.removeListener(receiver1_handle, Event::UPDATE)) << "EventDispatcher::removeListener returns false when no matching listener was removed";
	EXPECT_FALSE(dispatcher.hasListener(Event::UPDATE));
	
	auto receiver2_handle = dispatcher.addListener(std::bind(&CustomReceiver::customHandler, &receiver2, _1), Event::CHANGE);
	EXPECT_TRUE(dispatcher.removeListener(receiver2_handle, Event::CHANGE));
	EXPECT_FALSE(dispatcher.removeListener(receiver2_handle, Event::CHANGE)) << "A matching listener can only be removed once";
	EXPECT_FALSE(dispatcher.removeListener(receiver2_handle, Event::UPDATE)) << "EventDispatcher::removeListener returns false when no matching listener was found";
	EXPECT_FALSE(dispatcher.hasListener(Event::CHANGE));
}

TEST(EventDispatcher, Dispatch)
{
	EventDispatcher dispatcher;
	ExampleListener receiver1;
	ExampleListener receiver2;
	CustomReceiver receiver3;
	int context = 0;
	
	EXPECT_FALSE(dispatcher.dispatch(Event::create(Event::UPDATE)));
	
	auto handle = dispatcher.addListener([&](const EventRef event) { context = 42; }, Event::UPDATE);
	dispatcher.addListener(std::bind(&ExampleListener::handler, &receiver1, _1), Event::UPDATE);
	dispatcher.addListener(std::bind(&CustomReceiver::customHandler, &receiver3, _1), CustomEvent::CUSTOM);
	
	dispatcher.removeListener(handle, Event::UPDATE);
	dispatcher.dispatch(Event::create(Event::UPDATE));
	
	EXPECT_EQ(receiver1.param1, 42) << "Matching event handler should be triggered for event listener";
	EXPECT_EQ(receiver3.param1, "") << "Event handler should NOT be triggered for event listener registered to non-matching event ID";
	EXPECT_EQ(context, 0) << "Lambda function should NOT be triggered when registered to non-matching event ID";
	
	dispatcher.addListener(std::bind(&ExampleListener::handler, &receiver2, _1), CustomEvent::CUSTOM);
	dispatcher.dispatch(CustomEvent::create(CustomEvent::CUSTOM, "test_my_custom_event"));
	
	EXPECT_EQ(receiver2.param1, 42) << "Matching event handler should be triggered for event listener registered to dispatched event ID";
	EXPECT_EQ(receiver3.param1, "test_my_custom_event") << "Custom event data should be available for class member function event handler";
}

TEST(EventDispatcher, EventScalability)
{
	EventDispatcher dispatcher;
	Consumer receiver;
	int event_limit = 100000;
	
	dispatcher.addListener(std::bind(&Consumer::increment, &receiver, _1), Event::UPDATE);
	for (int i=0; i<event_limit; i++)
		dispatcher.dispatch(Event::create(Event::UPDATE));
	
	EXPECT_EQ(receiver.received_events, event_limit) << "Don't drop any events";
}

TEST(EventDispatcher, ListenerScalability)
{
	EventDispatcher dispatcher;
	std::array<Reducer,100000> reducers;
	
	for (auto reducer : reducers)
		dispatcher.addListener(std::bind(&Reducer::increment, &reducer, _1), Event::UPDATE);
	
	dispatcher.dispatch(Event::create(Event::UPDATE));
	EXPECT_EQ(Reducer::all_events, reducers.size()) << "Don't drop any listeners";
}
