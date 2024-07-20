#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Object/API.h>
#include <Engine/Job/JobThread.h>

namespace Hollow
{
	class ENGINE_API JobQueue : public API<JobQueue>
	{
	public:
		JobQueue() = default;
		~JobQueue() = default;


	private:
		Queue<JobThread> mJobQueue;
	};
}
