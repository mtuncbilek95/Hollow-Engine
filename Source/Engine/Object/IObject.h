#pragma once

#include <Engine/Core/Core.h>

namespace Hollow
{
	class IObject : public std::enable_shared_from_this<IObject>
	{
	public:
		virtual ~IObject() = default;

		template<typename T>
		FORCEINLINE SharedPtr<T> GetSharedPtrAs() { return std::static_pointer_cast<T>(shared_from_this()); }
		FORCEINLINE SharedPtr<IObject> GetSharedPtr() { return shared_from_this(); }

		virtual void Shutdown() {};
	};
}
