#include "VulkanRenderPass.h"

#include <Runtime/Vulkan/Texture/VulkanTextureUtils.h>
#include <Runtime/Vulkan/RenderPass/VulkanRenderPassUtils.h>

#include <Runtime/Vulkan/Texture/VulkanTextureView.h>

namespace Hollow
{
	VulkanRenderPass::VulkanRenderPass(const RenderPassDesc& desc, VulkanDevice* pDevice) : RenderPass(desc, pDevice), mVkDevice(pDevice->GetVkDevice()), mVkRenderPass(VK_NULL_HANDLE)
	{
		arrayList<VkAttachmentDescription> colorAttachments;

		for (const auto& attachment : desc.Attachments)
		{
			VkAttachmentDescription attachmentDesc = {};

			attachmentDesc.format = VulkanTextureUtils::GetVkTextureFormat(attachment.Format);
			attachmentDesc.samples = VulkanTextureUtils::GetVkSampleCount(attachment.SampleCount);
			attachmentDesc.loadOp = VulkanRenderPassUtils::GetVkAttachmentLoadOp(attachment.LoadOperation);
			attachmentDesc.storeOp = VulkanRenderPassUtils::GetVkAttachmentStoreOp(attachment.StoreOperation);
			attachmentDesc.stencilLoadOp = VulkanRenderPassUtils::GetVkAttachmentLoadOp(attachment.StencilLoadOperation);
			attachmentDesc.stencilStoreOp = VulkanRenderPassUtils::GetVkAttachmentStoreOp(attachment.StencilStoreOperation);
			attachmentDesc.initialLayout = VulkanTextureUtils::GetVkTextureMemoryLayout(attachment.InputLayout);
			attachmentDesc.finalLayout = VulkanTextureUtils::GetVkTextureMemoryLayout(attachment.OutputLayout);
			attachmentDesc.flags = 0;

			colorAttachments.push_back(attachmentDesc);
		}

		if (desc.DepthStencilAttachment.View != nullptr)
		{
			VkAttachmentDescription attachmentDesc = {};

			attachmentDesc.format = VulkanTextureUtils::GetVkTextureFormat(desc.DepthStencilAttachment.Format);
			attachmentDesc.samples = VulkanTextureUtils::GetVkSampleCount(desc.DepthStencilAttachment.SampleCount);
			attachmentDesc.loadOp = VulkanRenderPassUtils::GetVkAttachmentLoadOp(desc.DepthStencilAttachment.LoadOperation);
			attachmentDesc.storeOp = VulkanRenderPassUtils::GetVkAttachmentStoreOp(desc.DepthStencilAttachment.StoreOperation);
			attachmentDesc.stencilLoadOp = VulkanRenderPassUtils::GetVkAttachmentLoadOp(desc.DepthStencilAttachment.StencilLoadOperation);
			attachmentDesc.stencilStoreOp = VulkanRenderPassUtils::GetVkAttachmentStoreOp(desc.DepthStencilAttachment.StencilStoreOperation);
			attachmentDesc.initialLayout = VulkanTextureUtils::GetVkTextureMemoryLayout(desc.DepthStencilAttachment.InputLayout);
			attachmentDesc.finalLayout = VulkanTextureUtils::GetVkTextureMemoryLayout(desc.DepthStencilAttachment.OutputLayout);
			attachmentDesc.flags = 0;

			colorAttachments.push_back(attachmentDesc);
		}

		arrayList<VkAttachmentReference> colorAttachmentRefs;
		VkAttachmentReference depthStencilAttachmentRef = {};

		for (int i = 0; i < colorAttachments.size(); i++)
		{
			if (colorAttachments[i].format == VK_FORMAT_D32_SFLOAT_S8_UINT)
				continue;

			VkAttachmentReference attachmentRef = {};
			attachmentRef.attachment = i;
			attachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			colorAttachmentRefs.push_back(attachmentRef);
		}

		if (desc.DepthStencilAttachment.View != nullptr)
		{
			depthStencilAttachmentRef.attachment = colorAttachments.size() - 1;
			depthStencilAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		}

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = colorAttachmentRefs.size();
		subpass.pColorAttachments = colorAttachmentRefs.data();
		subpass.pDepthStencilAttachment = desc.DepthStencilAttachment.View != nullptr ? &depthStencilAttachmentRef : nullptr;

		// Create render pass
		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = static_cast<uint32_t>(colorAttachments.size());
		renderPassInfo.pAttachments = colorAttachments.data();
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 0;
		renderPassInfo.pDependencies = nullptr;

		CORE_ASSERT(vkCreateRenderPass(mVkDevice, &renderPassInfo, nullptr, &mVkRenderPass) == VK_SUCCESS, "VulkanRenderPass", "Failed to create render pass");

		arrayList<VkImageView> attachments;

		// Create framebuffers
		for (int i = 0; i < desc.Attachments.size(); i++)
		{
			VkImageView attachment = static_cast<VulkanTextureView*>(desc.Attachments[i].View)->GetVkTextureView();
			attachments.push_back(attachment);
		}

		if (desc.DepthStencilAttachment.View != nullptr)
		{
			VkImageView attachment = static_cast<VulkanTextureView*>(desc.DepthStencilAttachment.View)->GetVkTextureView();
			attachments.push_back(attachment);
		}

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = mVkRenderPass;
		framebufferInfo.attachmentCount = attachments.size();
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = desc.TargetSize.x;
		framebufferInfo.height = desc.TargetSize.y;
		framebufferInfo.layers = 1;

		CORE_ASSERT(vkCreateFramebuffer(mVkDevice, &framebufferInfo, nullptr, &mVkFramebuffer) == VK_SUCCESS, "RenderPass", "Failed to create framebuffer!");
	}

	void VulkanRenderPass::OnShutdown() noexcept
	{
		if (mVkFramebuffer != VK_NULL_HANDLE)
			vkDestroyFramebuffer(mVkDevice, mVkFramebuffer, nullptr);

		if (mVkRenderPass != VK_NULL_HANDLE)
			vkDestroyRenderPass(mVkDevice, mVkRenderPass, nullptr);

		mVkRenderPass = VK_NULL_HANDLE;
		mVkDevice = VK_NULL_HANDLE;

		CORE_LOG(HE_VERBOSE, "VulkanRenderPass", "Render pass destroyed successfully!");
	}
}