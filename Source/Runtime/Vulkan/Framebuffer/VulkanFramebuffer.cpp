#include "VulkanFramebuffer.h"

#include <Runtime/Vulkan/RenderPass/VulkanRenderPass.h>
#include <Runtime/Vulkan/Texture/VulkanTextureBuffer.h>

namespace Hollow
{
	VulkanFramebuffer::VulkanFramebuffer(const FramebufferDesc& desc, SharedPtr<VulkanDevice> device) : Framebuffer(desc, device), mFramebuffer(VK_NULL_HANDLE)
	{
		mLogicalDevice = device->GetVkDevice();

		DArray<VkImageView> attachments;
		for (auto& attachment : desc.Attachments)
			attachments.push_back(std::static_pointer_cast<VulkanTextureBuffer>(attachment)->GetVkTextureBuffer());

		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = std::static_pointer_cast<VulkanRenderPass>(desc.RPass)->GetVkRenderPass();
		framebufferInfo.attachmentCount = attachments.size();
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = desc.ImageSize.x;
		framebufferInfo.height = desc.ImageSize.y;
		framebufferInfo.layers = 1;

		CORE_ASSERT(vkCreateFramebuffer(device->GetVkDevice(), &framebufferInfo, nullptr, &mFramebuffer) == VK_SUCCESS, "VulkanFramebuffer", "Failed to create framebuffer!");
	}

	VulkanFramebuffer::~VulkanFramebuffer()
	{
		vkDestroyFramebuffer(mLogicalDevice, mFramebuffer, nullptr);
	}
}
