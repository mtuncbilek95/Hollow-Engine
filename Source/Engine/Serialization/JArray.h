#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Serialization/Core/JNode.h>

namespace Hollow
{
	class JArray : public JNode
	{
	public:
		JArray(const String& name) : JNode(name) {}
		~JArray() = default;

		NodeType GetType() const override { return NodeType::Array; }

		void AddNode(JNode* node) { mNodes.push_back(std::move(node)); }
		
		JNode* operator[](s64 index) { return mNodes[index]; }
		JNode* operator[](const String& name) { return GetNode(name); }

		JNode* GetNode(s64 index) { return mNodes[index]; }
		JNode* GetNode(const String& name)
		{
			for (auto& node : mNodes)
			{
				if (node->GetName() == name)
					return node;
			}

			CORE_LOG(HE_ERROR, "Node [%s] has not found.", name.c_str());
			return nullptr;
		}

		s64 GetSize() const { return mNodes.size(); }

		DArray<JNode*>& GetNodes() { return mNodes; }

	private:
		DArray<JNode*> mNodes;
	};
}
