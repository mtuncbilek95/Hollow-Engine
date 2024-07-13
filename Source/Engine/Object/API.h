#pragma once

#include <Engine/Core/Core.h>

namespace Hollow
{
	template<typename TObject>
	class ENGINE_API API
	{
	public:
		API() { mInstance = MakeShared<TObject>(); }
		virtual ~API() = default;

		static TObject* GetAPI()
		{
			CORE_ASSERT(mInstance != nullptr, "Manager Abstraction", "API is not initialized");
			return mInstance.get();
		}

	private:
		static inline SharedPtr<TObject> mInstance;
	};
}
