#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Queue/GraphicsQueueType.h>
#include <Engine/Graphics/Command/CmdPoolType.h>
#include <Engine/Graphics/Command/Core/GraphicsIndexType.h>
#include <Engine/Graphics/Command/Core/ResolveModeFlags.h>
#include <Engine/Graphics/Command/Core/AttachmentLoadOperation.h>
#include <Engine/Graphics/Command/Core/AttachmentStoreOperation.h>

#include <vulkan/vulkan.h>

namespace Hollow
{
	namespace VkUtils
	{
		static GraphicsQueueType GetQueueType(CmdPoolType type)
		{
			switch (type)
			{
			case CmdPoolType::Graphics:
				return GraphicsQueueType::Graphics;
			case CmdPoolType::Compute:
				return GraphicsQueueType::Compute;
			case CmdPoolType::Transfer:
				return GraphicsQueueType::Transfer;
			default:
				return GraphicsQueueType::Graphics;
			}
		}

		static VkIndexType GetIndexType(GraphicsIndexType type)
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

		static VkResolveModeFlagBits GetResolveMode(ResolveModeFlags flags)
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

		static VkAttachmentLoadOp GetVkAttachmentLoadOp(AttachmentLoadOperation op)
		{
			switch (op)
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

		static VkAttachmentStoreOp GetVkAttachmentStoreOp(AttachmentStoreOperation op)
		{
			switch (op)
			{
			case AttachmentStoreOperation::Store:
				return VK_ATTACHMENT_STORE_OP_STORE;
			case AttachmentStoreOperation::DontCare:
				return VK_ATTACHMENT_STORE_OP_DONT_CARE;
			default:
				return VK_ATTACHMENT_STORE_OP_STORE;
			}
		}
	}
}