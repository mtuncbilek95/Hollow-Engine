#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Core/ShareMode.h>
#include <Runtime/Graphics/Core/LogicOperation.h>
#include <Runtime/Graphics/Core/CompareOperation.h>
#include <Runtime/Graphics/Core/GraphicsIndexType.h>
#include <Runtime/Graphics/Core/ResolveModeFlags.h>
#include <Runtime/Graphics/Core/AttachmentLoadOperation.h>
#include <Runtime/Graphics/Core/AttachmentStoreOperation.h>

#include <vulkan/vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanCoreUtils
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

		static VkResolveModeFlagBits GetVkResolveModeFlagBits(ResolveModeFlags flags)
		{
			switch (flags)
			{
			case ResolveModeFlags::None:
				return VK_RESOLVE_MODE_NONE;
			case ResolveModeFlags::SampleZero:
				return VK_RESOLVE_MODE_SAMPLE_ZERO_BIT;
			case ResolveModeFlags::Average:
				return VK_RESOLVE_MODE_AVERAGE_BIT;
			case ResolveModeFlags::Min:
				return VK_RESOLVE_MODE_MIN_BIT;
			case ResolveModeFlags::Max:
				return VK_RESOLVE_MODE_MAX_BIT;
			default:
				return VK_RESOLVE_MODE_NONE;
			}
		}

		static VkAttachmentLoadOp GetVkAttachmentLoadOperation(AttachmentLoadOperation operation)
		{
			switch (operation)
			{
			case AttachmentLoadOperation::Load:
				return VK_ATTACHMENT_LOAD_OP_LOAD;
			case AttachmentLoadOperation::Clear:
				return VK_ATTACHMENT_LOAD_OP_CLEAR;
			case AttachmentLoadOperation::DontCare:
				return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			default:
				return VK_ATTACHMENT_LOAD_OP_LOAD;
			}
		}

		static VkAttachmentStoreOp GetVkAttachmentStoreOperation(AttachmentStoreOperation operation)
		{
			switch (operation)
			{
			case AttachmentStoreOperation::Store:
				return VK_ATTACHMENT_STORE_OP_STORE;
			case AttachmentStoreOperation::DontCare:
				return VK_ATTACHMENT_STORE_OP_DONT_CARE;
			case AttachmentStoreOperation::None:
				return VK_ATTACHMENT_STORE_OP_NONE;
			default:
				return VK_ATTACHMENT_STORE_OP_STORE;
			}
		}

		static VkPipelineRenderingCreateInfo CreatePipelineRenderingInfo()
		{
			VkFormat formats[] = { VK_FORMAT_B8G8R8_SRGB };
			VkPipelineRenderingCreateInfo renderingCreateInfo = {};
			renderingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
			renderingCreateInfo.pNext = nullptr;
			renderingCreateInfo.colorAttachmentCount = 1;
			renderingCreateInfo.pColorAttachmentFormats = formats;
			renderingCreateInfo.depthAttachmentFormat = VK_FORMAT_UNDEFINED;
			renderingCreateInfo.stencilAttachmentFormat = VK_FORMAT_UNDEFINED;
			renderingCreateInfo.viewMask = 0;

			return renderingCreateInfo;
		}

	public:
		VulkanCoreUtils() = delete;
		~VulkanCoreUtils() = delete;
	};
}