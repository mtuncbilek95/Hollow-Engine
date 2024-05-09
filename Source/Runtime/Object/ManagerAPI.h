#pragma once

#include <Runtime/Core/Definitions.h>
#include <Runtime/Core/StdFix.h>

namespace Hollow
{
	template<typename TObject>
	class RUNTIME_API ManagerAPI
	{
	public:
		ManagerAPI() = default;
		virtual ~ManagerAPI() = default;

		static TObject& GetInstance()
		{
			static TObject instance;
			return instance;
		}
	};
}
