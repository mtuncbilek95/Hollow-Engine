#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/RenderPass/RenderPassLoadOperation.h>
#include <Runtime/Graphics/RenderPass/RenderPassStoreOperation.h>
#include <Runtime/Graphics/Texture/TextureMemoryLayout.h>

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

		static VkImageLayout GetVkTextureMemoryLayout(TextureMemoryLayout layout)
		{
			switch (layout)
			{
			case TextureMemoryLayout::Unknown:
				return VK_IMAGE_LAYOUT_UNDEFINED;
			case TextureMemoryLayout::General:
				return VK_IMAGE_LAYOUT_GENERAL;
			case TextureMemoryLayout::ColorAttachment:
				return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			case TextureMemoryLayout::DepthStencilAttachment:
				return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			case TextureMemoryLayout::DepthAttachment:
				return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
			case TextureMemoryLayout::ShaderReadOnly:
				return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			case TextureMemoryLayout::TransferSource:
				return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			case TextureMemoryLayout::TransferDestination:
				return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			case TextureMemoryLayout::Present:
				return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
			default:
				return VK_IMAGE_LAYOUT_UNDEFINED;
			}
		}

	public:
		VulkanRenderPassUtils() = delete;
		~VulkanRenderPassUtils() = delete;
	};
}