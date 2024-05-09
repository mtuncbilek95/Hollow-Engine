#pragma once

#include <Runtime/Core/Core.h>

#include <Runtime/Platform/PlatformGuid.h>
#include <Runtime/Core/Guid.h>

namespace Hollow
{
	class RUNTIME_API Object
	{
	public:
		Object()
		{
			PlatformGuid::GenerateGuid(mResourceID);
		}
		virtual ~Object() = default;

		FORCEINLINE Guid GetResourceID() const { return mResourceID; }

	private:
		Guid mResourceID;
	};
}
