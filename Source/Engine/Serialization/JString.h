#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Serialization/Core/JNode.h>

namespace Hollow
{
	// DO NOT MAKE THAT JOKE!
	class JString : public JNode
	{
	public:
		JString(const String& name, const String& value) : JNode(name), mValue(value) {}
		~JString() = default;

		void SetValue(const String& value) { mValue = value; }

		FORCEINLINE const String& GetValue() const { return mValue; }
		NodeType GetType() const override { return NodeType::String; }

	private:
		String mValue;
	};
}
