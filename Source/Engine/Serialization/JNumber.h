#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Serialization/Core/JNode.h>

#pragma warning(disable: 4715)

namespace Hollow
{
	template<typename T>
	class JNumber : public JNode
	{
	public:
		JNumber(const String& name, T value) : JNode(name), mValue(value) {}
		~JNumber() = default;

		void SetValue(T value) { mValue = value; }

		FORCEINLINE T GetValue() const { return mValue; }
		NodeType GetType() const override {
			if (sizeof(T) == sizeof(i64))
				return NodeType::Int;
			else if (sizeof(T) == sizeof(f64))
				return NodeType::Float;
			else if (sizeof(T) == sizeof(u64))
				return NodeType::UInt;
		}

	private:
		T mValue;
	};
}

#pragma warning(default: 4715)