#pragma once

#include <Engine/Core/Core.h>

#include <thread>

namespace Hollow
{
	class JobThread
	{
	public:
		JobThread();
		~JobThread();

		void Start() { mThread = std::thread(&JobThread::Run, this); }
		void Run() { while (true) { /* Do work */ } }
		void Join() { mThread.join(); }
		std::thread::id GetId() const { return mThread.get_id(); }

	private:
		std::thread mThread;
	};
}
