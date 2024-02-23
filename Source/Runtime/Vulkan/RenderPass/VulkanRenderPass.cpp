#include "VulkanRenderPass.h"

#include <Runtime/Vulkan/Texture/VulkanTextureUtils.h>
#include <Runtime/Vulkan/RenderPass/VulkanRenderPassUtilts.h>

#include <Runtime/Vulkan/Texture/VulkanTextureView.h>

namespace Hollow
{
	VulkanRenderPass::VulkanRenderPass(const RenderPassDesc& desc, VulkanDevice* pDevice) : RenderPass(desc, pDevice)
	{
		mVkLogicalDevice = pDevice->GetVkLogicalDevice();

		// Create color attachments
		Array<VkAttachmentDescription> colorAttachments;
		for (uint32 attachmentIndex = 0; attachmentIndex < desc.ColorAttachments.size(); attachmentIndex++)
		{
			VkAttachmentDescription colorAttachment = {};
			colorAttachment.format = VulkanTextureUtils::GetVkFormat(desc.ColorAttachments[attachmentIndex].Format);
			colorAttachment.samples = VulkanTextureUtils::GetVkSampleCount(desc.ColorAttachments[attachmentIndex].SampleCount);
			colorAttachment.loadOp = VulkanRenderPassUtils::GetVkLoadOp(desc.ColorAttachments[attachmentIndex].ColorLoadOp);
			colorAttachment.storeOp = VulkanRenderPassUtils::GetVkStoreOp(desc.ColorAttachments[attachmentIndex].ColorStoreOp);
			colorAttachment.stencilLoadOp = VulkanRenderPassUtils::GetVkLoadOp(desc.ColorAttachments[attachmentIndex].DepthLoadOp);
			colorAttachment.stencilStoreOp = VulkanRenderPassUtils::GetVkStoreOp(desc.ColorAttachments[attachmentIndex].DepthStoreOp);
			colorAttachment.initialLayout = VulkanRenderPassUtils::GetVkTextureLayout(desc.ColorAttachments[attachmentIndex].InputLayout);
			colorAttachment.finalLayout = VulkanRenderPassUtils::GetVkTextureLayout(desc.ColorAttachments[attachmentIndex].OutputLayout);
			colorAttachment.flags = VkAttachmentDescriptionFlags();

			colorAttachments.push_back(colorAttachment);
		}

		// Create depth attachment
		if (desc.HasDepthStencilAttachment)
		{
			VkAttachmentDescription depthAttachment = {};
			depthAttachment.format = VulkanTextureUtils::GetVkFormat(desc.DepthStencilAttachment.Format);
			depthAttachment.samples = VulkanTextureUtils::GetVkSampleCount(desc.DepthStencilAttachment.SampleCount);
			depthAttachment.loadOp = VulkanRenderPassUtils::GetVkLoadOp(desc.DepthStencilAttachment.DepthLoadOp);
			depthAttachment.storeOp = VulkanRenderPassUtils::GetVkStoreOp(desc.DepthStencilAttachment.DepthStoreOp);
			depthAttachment.stencilLoadOp = VulkanRenderPassUtils::GetVkLoadOp(desc.DepthStencilAttachment.DepthLoadOp);
			depthAttachment.stencilStoreOp = VulkanRenderPassUtils::GetVkStoreOp(desc.DepthStencilAttachment.DepthStoreOp);
			depthAttachment.initialLayout = VulkanRenderPassUtils::GetVkTextureLayout(desc.DepthStencilAttachment.InputLayout);
			depthAttachment.finalLayout = VulkanRenderPassUtils::GetVkTextureLayout(desc.DepthStencilAttachment.OutputLayout);
			depthAttachment.flags = VkAttachmentDescriptionFlags();

			colorAttachments.push_back(depthAttachment);
		}

		// Create attachment references
		Array<VkAttachmentReference> colorAttachmentRefs;
		for (uint32 attachmentIndex = 0; attachmentIndex < desc.ColorAttachments.size(); attachmentIndex++)
		{
			VkAttachmentReference colorAttachmentRef = {};
			colorAttachmentRef.attachment = attachmentIndex;
			colorAttachmentRef.layout = colorAttachments[attachmentIndex].initialLayout;

			colorAttachmentRefs.push_back(colorAttachmentRef);
		}

		// Create subpass
		VkSubpassDescription subpass = {};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.preserveAttachmentCount = 0;
		subpass.pPreserveAttachments = nullptr;

		subpass.colorAttachmentCount = desc.ColorAttachments.size();
		subpass.pColorAttachments = colorAttachmentRefs.data();

		subpass.inputAttachmentCount = 0;
		subpass.pInputAttachments = nullptr;

		subpass.pDepthStencilAttachment = desc.HasDepthStencilAttachment ? &colorAttachmentRefs[desc.ColorAttachments.size()] : nullptr;

		subpass.pResolveAttachments = nullptr;

		// Create render pass
		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = colorAttachments.size();
		renderPassInfo.pAttachments = colorAttachments.data();
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 0;
		renderPassInfo.pDependencies = nullptr;
		renderPassInfo.pNext = nullptr;

		DEV_ASSERT(vkCreateRenderPass(mVkLogicalDevice, &renderPassInfo, nullptr, &mVkRenderPass) == VK_SUCCESS, "VulkanRenderPass", "Failed to create render pass");

		// Create framebuffer
		Array<VkImageView> textuteViews;

		for(uint32 attachmentIndex = 0; attachmentIndex < desc.ColorAttachments.size(); attachmentIndex++)
		{
			VulkanTextureView* pView = reinterpret_cast<VulkanTextureView*>(desc.ColorAttachments[attachmentIndex].pTextureView.get());
			textuteViews.push_back(pView->GetVkTextureView());
		}

		if(desc.HasDepthStencilAttachment)
		{
			VulkanTextureView* pView = reinterpret_cast<VulkanTextureView*>(desc.DepthStencilAttachment.pTextureView.get());
			textuteViews.push_back(pView->GetVkTextureView());
		}

		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = mVkRenderPass;
		framebufferInfo.attachmentCount = textuteViews.size();
		framebufferInfo.pAttachments = textuteViews.data();
		framebufferInfo.width = desc.FramebufferSize.x;
		framebufferInfo.height = desc.FramebufferSize.y;
		framebufferInfo.layers = 1;
		framebufferInfo.flags = VkFramebufferCreateFlags();
		framebufferInfo.pNext = nullptr;

		DEV_ASSERT(vkCreateFramebuffer(mVkLogicalDevice, &framebufferInfo, nullptr, &mVkFramebuffer) == VK_SUCCESS, "VulkanRenderPass", "Failed to create framebuffer");
	}

	void VulkanRenderPass::OnShutdown() noexcept
	{
		if (mVkFramebuffer != VK_NULL_HANDLE)
		{
			vkDestroyFramebuffer(mVkLogicalDevice, mVkFramebuffer, nullptr);
			mVkFramebuffer = VK_NULL_HANDLE;
		}

		if (mVkRenderPass != VK_NULL_HANDLE)
		{
			vkDestroyRenderPass(mVkLogicalDevice, mVkRenderPass, nullptr);
			mVkRenderPass = VK_NULL_HANDLE;
		}
	}
}