#pragma once

#include <Runtime/Core/Core.h>

#include <Runtime/Graphics/Common/CompareOperation.h>
#include <Runtime/Graphics/Common/LogicOperation.h>
#include <Runtime/Graphics/Common/PresentMode.h>
#include <Runtime/Graphics/Common/ShareMode.h>

#include <vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanCommonUtils
	{
	public:
		static VkCompareOp GetVkCompareOperation(CompareOperation operation)
		{
			switch (operation)
			{
			case CompareOperation::Never:
				return VK_COMPARE_OP_NEVER;
			case CompareOperation::Less:
				return VK_COMPARE_OP_LESS;
			case CompareOperation::Equal:
				return VK_COMPARE_OP_EQUAL;
			case CompareOperation::LessEqual:
				return VK_COMPARE_OP_LESS_OR_EQUAL;
			case CompareOperation::Greater:
				return VK_COMPARE_OP_GREATER;
			case CompareOperation::NotEqual:
				return VK_COMPARE_OP_GREATER;
			case CompareOperation::GreaterEqual:
				return VK_COMPARE_OP_GREATER_OR_EQUAL;
			case CompareOperation::Always:
				return VK_COMPARE_OP_ALWAYS;
			default:
				return VK_COMPARE_OP_NEVER;
			}
		}

		static VkLogicOp GetVkLogicOperation(LogicOperation operation)
		{
			switch (operation)
			{
			case LogicOperation::Clear:
				return VK_LOGIC_OP_CLEAR;
			case LogicOperation::Set:
				return VK_LOGIC_OP_SET;
			case LogicOperation::Copy:
				return VK_LOGIC_OP_COPY;
			case LogicOperation::CopyInverted:
				return VK_LOGIC_OP_COPY_INVERTED;
			case LogicOperation::NoOp:
				return VK_LOGIC_OP_NO_OP;
			case LogicOperation::Invert:
				return VK_LOGIC_OP_INVERT;
			case LogicOperation::And:
				return VK_LOGIC_OP_AND;
			case LogicOperation::NotAnd:
				return VK_LOGIC_OP_NAND;
			case LogicOperation::Or:
				return VK_LOGIC_OP_OR;
			case LogicOperation::Nor:
				return VK_LOGIC_OP_NOR;
			case LogicOperation::Xor:
				return VK_LOGIC_OP_XOR;
			case LogicOperation::Equal:
				return VK_LOGIC_OP_EQUIVALENT;
			case LogicOperation::AndReverse:
				return VK_LOGIC_OP_AND_REVERSE;
			case LogicOperation::AndInverted:
				return VK_LOGIC_OP_AND_INVERTED;
			case LogicOperation::OrReverse:
				return VK_LOGIC_OP_OR_REVERSE;
			case LogicOperation::OrInverted:
				return VK_LOGIC_OP_OR_INVERTED;
			default:
				return VK_LOGIC_OP_CLEAR;

			}
		}

	public:
		VulkanCommonUtils() = delete;
		~VulkanCommonUtils() = delete;
	};
}