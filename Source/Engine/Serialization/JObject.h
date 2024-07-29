#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Serialization/Core/JNode.h>

namespace Hollow
{
	class JObject : public JNode
	{
		friend class JSerializer;
	public:
		JObject(const String& name) : JNode(name) {}
		~JObject()
		{
			for (auto& child : mChildren)
				delete child.second;
		}

		NodeType GetType() const override { return NodeType::Object; }

		void AddChild(const String& name, JNode* child) { mChildren.insert({ name, std::move(child) }); }

		JNode* GetChild(const String& name)
		{
			auto it = mChildren.find(name);
			if (it != mChildren.end())
				return it->second;

			CORE_LOG(HE_ERROR, "Child node [%s] has not found.", name.c_str());
			return nullptr;
		}

		JNode* operator[](const String& name) { return GetChild(name); }
		JNode* At(const String& name) { return GetChild(name); }

		HashMap<String, JNode*>& GetChildren() { return mChildren; }

	private:
		HashMap<String, JNode*> mChildren;
	};
}
