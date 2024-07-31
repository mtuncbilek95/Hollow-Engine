#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Platform/PlatformGuid.h>

namespace Hollow
{
	class IResource
	{
	public:
		IResource()
		{
			PlatformGuid::GenerateGuid(mGuid);
		}
		virtual ~IResource() = default;

		FORCEINLINE const Guid& GetGuid() const { return mGuid; }

	private:
		Guid mGuid;
	};
}
