#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Core/CompareOperation.h>
#include <Engine/Graphics/Core/LogicOperation.h>

#include <vulkan/vulkan.h>

namespace Hollow
{
	namespace Utils
	{
		static VkCompareOp GetVkCompareOp(CompareOperation operation)
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
				return VK_COMPARE_OP_NOT_EQUAL;
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
	}
}
