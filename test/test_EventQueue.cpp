#include <chrono>
#include <functional>
#include <string>
#include <thread>

#include "gtest/gtest.h"

#include "Events.h"
#include "EventQueue.h"
#include "EventTestHelpers.hpp"

using namespace std::placeholders;

// defined in test_EventDispatcher.cpp:
//   std::hash<std::string> str_hash;
//   const EventId CustomEvent::CUSTOM = str_hash( "ReceiverCustomEvent" );
//

class Producer {
public:
	int all_events;
	void increment(const EventRef event) {
		all_events++;
	}
};

TEST(EventQueue, Destruction)
{
	EventQueue* queue = new EventQueue();
	ExampleListener receiver1;
	
	queue->addListener(std::bind(&ExampleListener::handler, &receiver1, _1), Event::UPDATE);
	queue->enqueue(Event::create(Event::UPDATE));
	EXPECT_EQ(receiver1.param1, 0);
	queue->update();
	
	EXPECT_NO_THROW(delete queue);
	
	EXPECT_EQ(receiver1.param1, 42);
}

TEST(EventQueue, AddListeners)
{
	EventQueue queue;
	ExampleListener receiver1;
	CustomReceiver receiver2;
	int context = 0;
	
	queue.addListener(std::bind(&ExampleListener::handler, &receiver1, _1), Event::UPDATE);
	queue.addListener(std::bind(&CustomReceiver::customHandler, &receiver2, _1), Event::UPDATE);
	queue.addListener([&](const EventRef event) { context = 42; }, Event::UPDATE);
	
	EXPECT_NO_THROW(queue.enqueue(Event::create(Event::UPDATE))) << "enqueue method should not throw exception";
	queue.update();
	
	EXPECT_EQ(receiver1.param1, 42) << "Public class method event handler should be triggered";
	EXPECT_EQ(receiver2.param1, "") << "Event handler method that applies type downcast should safely act as a no-op";
	EXPECT_EQ(context, 42) << "Lambda event handlers should work as normal";
}

TEST(EventQueue, HasListeners)
{
	EventQueue queue;
	ExampleListener receiver1;
	CustomReceiver receiver2;
	
	EXPECT_FALSE(queue.hasListener(Event::UPDATE)) << "EventQueue::hasListener returns false without any matching listeners";
	queue.addListener(std::bind(&ExampleListener::handler, &receiver1, _1), Event::UPDATE);
	EXPECT_TRUE(queue.hasListener(Event::UPDATE)) << "EventQueue::hasListener returns true with matching listener";
	
	EXPECT_FALSE(queue.hasListener(Event::CHANGE)) << "EventQueue::hasListener respects input event type";
	queue.addListener(std::bind(&CustomReceiver::customHandler, &receiver2, _1), Event::CHANGE);
	EXPECT_TRUE(queue.hasListener(Event::CHANGE)) << "EventQueue::hasListener respects input event type";
}

TEST(EventQueue, RemoveListener)
{
	EventQueue queue;
	ExampleListener receiver1;
	CustomReceiver receiver2;
	
	auto receiver1_handle = queue.addListener(std::bind(&ExampleListener::handler, &receiver1, _1), Event::UPDATE);
	EXPECT_TRUE(queue.removeListener(receiver1_handle, Event::UPDATE)) << "EventQueue::removeListener returns true when matching listener was removed";
	EXPECT_FALSE(queue.removeListener(receiver1_handle, Event::UPDATE)) << "EventQueue::removeListener returns false when no matching listener was removed";
	EXPECT_FALSE(queue.hasListener(Event::UPDATE));
	
	auto receiver2_handle = queue.addListener(std::bind(&CustomReceiver::customHandler, &receiver2, _1), Event::CHANGE);
	EXPECT_TRUE(queue.removeListener(receiver2_handle, Event::CHANGE));
	EXPECT_FALSE(queue.removeListener(receiver2_handle, Event::CHANGE)) << "A matching listener can only be removed once";
	EXPECT_FALSE(queue.removeListener(receiver2_handle, Event::UPDATE)) << "EventQueue::removeListener returns false when no matching listener was found";
	EXPECT_FALSE(queue.hasListener(Event::CHANGE));
}

TEST(EventQueue, EventDispatch)
{
	EventQueue queue;
	ExampleListener receiver1;
	ExampleListener receiver2;
	CustomReceiver receiver3;
	int context = 0;
	
	EXPECT_TRUE(queue.enqueue(Event::create(Event::UPDATE)));
	queue.update();
	
	auto handle = queue.addListener([&](const EventRef event) { context = 42; }, Event::UPDATE);
	queue.addListener(std::bind(&ExampleListener::handler, &receiver1, _1), Event::UPDATE);
	queue.addListener(std::bind(&CustomReceiver::customHandler, &receiver3, _1), CustomEvent::CUSTOM);
	
	queue.removeListener(handle, Event::UPDATE);
	queue.enqueue(Event::create(Event::UPDATE));
	queue.update();
	
	EXPECT_EQ(receiver1.param1, 42) << "Matching event handler should be triggered for event listener";
	EXPECT_EQ(receiver3.param1, "") << "Event handler should NOT be triggered for event listener registered to non-matching event ID";
	EXPECT_EQ(context, 0) << "Lambda function should NOT be triggered when registered to non-matching event ID";
	
	queue.addListener(std::bind(&ExampleListener::handler, &receiver2, _1), CustomEvent::CUSTOM);
	queue.enqueue(CustomEvent::create(CustomEvent::CUSTOM, "test_my_custom_event"));
	queue.update();
	
	EXPECT_EQ(receiver2.param1, 42) << "Matching event handler should be triggered for event listener registered to dispatched event ID";
	EXPECT_EQ(receiver3.param1, "test_my_custom_event") << "Custom event data should be available for class member function event handler";
}

TEST(EventQueue, ProducerConsumer)
{
	int count = 0;
	EventListener receiver = [&](auto event) mutable { count++; };
	
	int event_limit = 1000;
	std::thread event_producer([&](){
		EventQueue queue;
		queue.addListener(receiver, Event::UPDATE);
		for (int i=0; i<event_limit; i++)
			queue.enqueue(Event::create(Event::UPDATE));
		queue.update();
	});
	
	event_producer.join();
	
	EXPECT_EQ(event_limit,count);
}

TEST(EventQueue, ConsumerProducer)
{
	EventQueue queue;
	int event_limit = 1000;
	for (int i=0; i<event_limit; i++)
		queue.enqueue(Event::create(Event::UPDATE));
	
	int count = 0;
	queue.addListener([&](auto event) mutable { count++; }, Event::UPDATE);
	
	std::thread event_producer([&](){
		while (count < event_limit) {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	});
	
	queue.update();
	
	event_producer.join();
	
	EXPECT_EQ(event_limit,count);
}

