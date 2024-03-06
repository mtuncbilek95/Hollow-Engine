#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	template<typename API>
	class RUNTIME_API Manager
	{
	public:
		Manager() = default;
		~Manager() = default;

		static API& GetInstance()
		{
			static API instance;
			return instance;
		}
	};
}
