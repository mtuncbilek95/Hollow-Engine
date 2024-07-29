#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Serialization/Core/JsonType.h>


namespace Hollow
{
	// Root node of the JSON tree
	class JNode
	{
	public:
		JNode(const String& name = "") : mName(name) {}
		~JNode() = default;

		const String& GetName() const { return mName; }
		virtual NodeType GetType() const = 0;

		template<typename T>
		T* Cast()
		{
			return static_cast<T*>(this);
		}

	private:
		String mName;
	};
}