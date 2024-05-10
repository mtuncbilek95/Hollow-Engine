#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Common/ShareMode.h>
#include <Runtime/Graphics/Common/LogicOperation.h>
#include <Runtime/Graphics/Common/CompareOperation.h>
#include <Runtime/Graphics/Common/GraphicsIndexType.h>

#include <vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanCommonUtils
	{
	public:
		static VkSharingMode GetVkSharingMode(ShareMode mode)
		{
			switch (mode)
			{
			case ShareMode::Exclusive:
				return VK_SHARING_MODE_EXCLUSIVE;
			case ShareMode::Concurrent:
				return VK_SHARING_MODE_CONCURRENT;
			default:
				return VK_SHARING_MODE_EXCLUSIVE;
			}
		}

		static VkLogicOp GetVkLogicOperation(LogicOperation operation)
		{
			switch (operation)
			{
			case LogicOperation::Clear:
				return VK_LOGIC_OP_CLEAR;
			case LogicOperation::And:
				return VK_LOGIC_OP_AND;
			case LogicOperation::AndReverse:
				return VK_LOGIC_OP_AND_REVERSE;
			case LogicOperation::Copy:
				return VK_LOGIC_OP_COPY;
			case LogicOperation::AndInverted:
				return VK_LOGIC_OP_AND_INVERTED;
			case LogicOperation::NoOp:
				return VK_LOGIC_OP_NO_OP;
			case LogicOperation::Xor:
				return VK_LOGIC_OP_XOR;
			case LogicOperation::Or:
				return VK_LOGIC_OP_OR;
			case LogicOperation::Nor:
				return VK_LOGIC_OP_NOR;
			case LogicOperation::Equal:
				return VK_LOGIC_OP_EQUIVALENT;
			case LogicOperation::Invert:
				return VK_LOGIC_OP_INVERT;
			case LogicOperation::OrReverse:
				return VK_LOGIC_OP_OR_REVERSE;
			case LogicOperation::CopyInverted:
				return VK_LOGIC_OP_COPY_INVERTED;
			case LogicOperation::OrInverted:
				return VK_LOGIC_OP_OR_INVERTED;
			case LogicOperation::NotAnd:
				return VK_LOGIC_OP_NAND;
			case LogicOperation::Set:
				return VK_LOGIC_OP_SET;
			default:
				return VK_LOGIC_OP_CLEAR;
			}
		}

		static VkCompareOp GetVkCompareOperation(CompareOperation operation)
		{
			switch (operation)
			{
			case Hollow::CompareOperation::Never:
				return VK_COMPARE_OP_NEVER;
			case Hollow::CompareOperation::Less:
				return VK_COMPARE_OP_LESS;
			case Hollow::CompareOperation::Equal:
				return VK_COMPARE_OP_EQUAL;
			case Hollow::CompareOperation::LessEqual:
				return VK_COMPARE_OP_LESS_OR_EQUAL;
			case Hollow::CompareOperation::Greater:
				return VK_COMPARE_OP_GREATER;
			case Hollow::CompareOperation::NotEqual:
				return VK_COMPARE_OP_NOT_EQUAL;
			case Hollow::CompareOperation::GreaterEqual:
				return VK_COMPARE_OP_GREATER_OR_EQUAL;
			case Hollow::CompareOperation::Always:
				return VK_COMPARE_OP_ALWAYS;
			default:
				return VK_COMPARE_OP_NEVER;
			}
		}

		static VkIndexType GetVkIndexType(GraphicsIndexType type)
		{
			switch (type)
			{
			case GraphicsIndexType::Index16:
				return VK_INDEX_TYPE_UINT16;
			case GraphicsIndexType::Index32:
				return VK_INDEX_TYPE_UINT32;
			default:
				return VK_INDEX_TYPE_UINT16;
			}

		}

	public:
		VulkanCommonUtils() = delete;
		~VulkanCommonUtils() = delete;
	};
}