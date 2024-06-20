#pragma once

#include <Runtime/Core/Core.h>

#include <Runtime/Platform/PlatformGuid.h>
#include <Runtime/Core/Guid.h>

namespace Hollow
{
	class RUNTIME_API Object : public std::enable_shared_from_this<Object>
	{
	public:
		Object()
		{
			PlatformGuid::GenerateGuid(mResourceID);
		}
		virtual ~Object() = default;

		FORCEINLINE SharedPtr<Object> GetSharedPtr() { return shared_from_this(); }
		FORCEINLINE Guid GetResourceID() const { return mResourceID; }

		virtual void ResetObject() {}

	private:
		Guid mResourceID;
	};
}
