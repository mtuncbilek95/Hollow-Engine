#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	// Base class for the event types. If you need to create a new event type, you should inherit from this class.
	template<typename Event0>
	class RUNTIME_API Event
	{
		typedef Event0 MainEvent;
	public:
		Event() = default;
		Event(MainEvent type, const String& name = "") : mEventType(type), mEventName(name) {}
		~Event() = default;

		MainEvent GetEventType() const { return mEventType; }
		const String& GetEventName() const { return mEventName; }

		template<typename Event1>
		Event1 ToType() { return static_cast<Event1>(MainEvent); }

	private:
		MainEvent mEventType;
		String mEventName;
	};
}
