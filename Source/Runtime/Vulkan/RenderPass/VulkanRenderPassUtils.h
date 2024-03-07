#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/RenderPass/RenderPassLoadOperation.h>
#include <Runtime/Graphics/RenderPass/RenderPassStoreOperation.h>

#include <vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanRenderPassUtils
	{
	public:
		static VkAttachmentLoadOp GetVkAttachmentLoadOp(RenderPassLoadOperation loadOp)
		{
			switch (loadOp)
			{
			case RenderPassLoadOperation::Load:
				return VK_ATTACHMENT_LOAD_OP_LOAD;
			case RenderPassLoadOperation::Clear:
				return VK_ATTACHMENT_LOAD_OP_CLEAR;
			case RenderPassLoadOperation::Ignore:
				return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			default:
				return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			}
		}

		static VkAttachmentStoreOp GetVkAttachmentStoreOp(RenderPassStoreOperation storeOp)
		{
			switch (storeOp)
			{
			case RenderPassStoreOperation::Store:
				return VK_ATTACHMENT_STORE_OP_STORE;
			case RenderPassStoreOperation::Ignore:
				return VK_ATTACHMENT_STORE_OP_DONT_CARE;
			default:
				return VK_ATTACHMENT_STORE_OP_DONT_CARE;
			}
		}

	public:
		VulkanRenderPassUtils() = delete;
		~VulkanRenderPassUtils() = delete;
	};
}