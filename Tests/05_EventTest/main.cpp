#include <Runtime/Event/EventHandler.h>
#include <Runtime/Event/Event.h>

using namespace Hollow;

#include <iostream>

class TestEvent : public Event<int>
{
public:
	TestEvent() : Event(0, "TestEvent") {}
	~TestEvent() = default;
};

class Test
{
public:
	Test()
	{
		mEventHandler.AddListener([this](const TestEvent& event) 
			{ 
				std::cout << "TestEvent received" << std::endl; 
			});
	}

	void Run()
	{
		TestEvent event;
		mEventHandler.Dispatch(event);
	}

	~Test() = default;

public:
	EventHandler<TestEvent> mEventHandler;
};
int main()
{

	Test bTest;

	bTest.Run();

	return 0;
}
