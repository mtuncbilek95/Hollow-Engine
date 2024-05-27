#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Event/Event.h>

#include <functional>

namespace Hollow
{
	template<typename T>
	using EventBlock = std::function<void(const T&)>;

	template<typename EventType>
	class RUNTIME_API EventHandler
	{
	public:
		EventHandler() = default;
		~EventHandler() = default;

		void AddListener(const EventBlock<EventType>& listener) { mListeners.push_back(listener); }
		void RemoveListener(EventBlock<EventType>& listener) 
		{ 
			mListeners.erase(
				std::remove_if(mListeners.begin(), mListeners.end(),
					[&]( EventBlock<EventType>& l) { return l.target<void(EventType&)>() == listener.target<void(EventType&)>(); }),
				mListeners.end()
			); 
		}

		void Dispatch(const EventType& event)
		{
			for (auto& listener : mListeners)
				listener(event);
		}

	private:
		ArrayList<EventBlock<EventType>> mListeners;
	};
}
