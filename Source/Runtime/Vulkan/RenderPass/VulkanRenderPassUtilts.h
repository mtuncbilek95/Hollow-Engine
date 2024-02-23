#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/RenderPass/RenderPassLoadOperation.h>
#include <Runtime/Graphics/RenderPass/RenderPassStoreOperation.h>
#include <Runtime/Graphics/Texture/TextureLayout.h>

#include <vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanRenderPassUtils
	{
	public:
		static VkAttachmentLoadOp GetVkLoadOperation(RenderPassLoadOperation operation)
		{
			switch (operation)
			{
			case RenderPassLoadOperation::Load:
				return VK_ATTACHMENT_LOAD_OP_LOAD;
			case RenderPassLoadOperation::Clear:
				return VK_ATTACHMENT_LOAD_OP_CLEAR;
			case RenderPassLoadOperation::Ignore:
				return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			case RenderPassLoadOperation::None:
				return VK_ATTACHMENT_LOAD_OP_NONE_EXT;
			default:
				return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			}
		}

		static VkAttachmentStoreOp GetVkStoreOperation(RenderPassStoreOperation operation)
		{
			switch (operation)
			{
			case RenderPassStoreOperation::Store:
				return VK_ATTACHMENT_STORE_OP_STORE;
			case RenderPassStoreOperation::Ignore:
				return VK_ATTACHMENT_STORE_OP_DONT_CARE;
			case RenderPassStoreOperation::None:
				return VK_ATTACHMENT_STORE_OP_NONE_EXT;
			default:
				return VK_ATTACHMENT_STORE_OP_DONT_CARE;
			}
		}

		static VkImageLayout GetVkTextureLayout(const TextureLayout memoryLayout) noexcept
		{
			switch (memoryLayout)
			{
			case TextureLayout::Unknown:
				return VK_IMAGE_LAYOUT_UNDEFINED;
			case TextureLayout::General:
				return VK_IMAGE_LAYOUT_GENERAL;
			case TextureLayout::ColorAttachment:
				return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			case TextureLayout::DepthStencilAttachment:
				return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
			case TextureLayout::ShaderReadOnly:
				return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			case TextureLayout::TransferSource:
				return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			case TextureLayout::TransferDestination:
				return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			case TextureLayout::DepthAttachment:
				return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			case TextureLayout::Present:
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