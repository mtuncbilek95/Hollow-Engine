#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Serialization/Core/JNode.h>

namespace Hollow
{
	class JBool : public JNode
	{
	public:
		JBool(const String& name, bool value) : JNode(name), mValue(value) {}
		~JBool() = default;

		void SetValue(bool value) { mValue = value; }

		FORCEINLINE bool GetValue() const { return mValue; }
		NodeType GetType() const override { return NodeType::Boolean; }

	private:
		bool mValue;
	};
}
