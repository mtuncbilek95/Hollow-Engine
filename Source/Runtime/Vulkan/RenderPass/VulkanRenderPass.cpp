#include "VulkanRenderPass.h"

#include <Runtime/Vulkan/Texture/VulkanTextureUtils.h>
#include <Runtime/Vulkan/RenderPass/VulkanRenderPassUtils.h>

#include <Runtime/Vulkan/Texture/VulkanTextureView.h>

namespace Hollow
{
	VulkanRenderPass::VulkanRenderPass(const RenderPassDesc& desc, VulkanDevice* pDevice) : RenderPass(desc, pDevice), mVkDevice(pDevice->GetVkDevice()),
		mVkRenderPass(VK_NULL_HANDLE), mVkFramebuffer(VK_NULL_HANDLE)
	{
		//Create color attachment descs
		Array<VkAttachmentDescription> attachments(desc.ColorAttachments.size());
		for (unsigned char i = 0; i < desc.ColorAttachments.size(); i++)
		{
			const RenderPassAttachmentDesc& attachmentDesc = desc.ColorAttachments[i];
			VkAttachmentDescription attachment = {};
			attachment.format = VulkanTextureUtils::GetVkTextureFormat(attachmentDesc.Format);
			attachment.samples = (VkSampleCountFlagBits)VulkanTextureUtils::GetVkSampleCount(attachmentDesc.SampleCount);
			attachment.loadOp = VulkanRenderPassUtils::GetVkAttachmentLoadOp(attachmentDesc.ColorLoadOperation);
			attachment.storeOp = VulkanRenderPassUtils::GetVkAttachmentStoreOp(attachmentDesc.ColorStoreOperation);
			attachment.initialLayout = VulkanRenderPassUtils::GetVkTextureMemoryLayout(attachmentDesc.InputLayout);
			attachment.finalLayout = VulkanRenderPassUtils::GetVkTextureMemoryLayout(attachmentDesc.OutputLayout);
			attachment.stencilLoadOp = VulkanRenderPassUtils::GetVkAttachmentLoadOp(attachmentDesc.StencilLoadOperation);
			attachment.stencilStoreOp = VulkanRenderPassUtils::GetVkAttachmentStoreOp(attachmentDesc.StencilStoreOperation);

			attachments[i] = attachment;
		}

		//Create attachment references
		VkAttachmentReference attachmentReferences[16];
		for (unsigned char i = 0; i < attachments.size(); i++)
		{
			const VkAttachmentDescription& attachment = attachments[i];

			VkAttachmentReference reference = {};
			reference.attachment = i;
			reference.layout = attachment.initialLayout;
			attachmentReferences[i] = reference;
		}

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.preserveAttachmentCount = 0;
		subpass.pPreserveAttachments = nullptr;

		subpass.colorAttachmentCount = desc.ColorAttachments.size();
		subpass.pColorAttachments = attachmentReferences;

		subpass.inputAttachmentCount = 0;
		subpass.pInputAttachments = nullptr;

		subpass.pDepthStencilAttachment = desc.pDepthStencilAttachment != nullptr ? &attachmentReferences[desc.ColorAttachments.size()] : nullptr;

		subpass.pResolveAttachments = nullptr;

		VkRenderPassCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		info.flags = VkRenderPassCreateFlags();
		info.attachmentCount = attachments.size();
		info.pAttachments = attachments.data();
		info.subpassCount = 1;
		info.pSubpasses = &subpass;
		info.dependencyCount = 0;
		info.pDependencies = nullptr;
		info.pNext = nullptr;

		DEV_ASSERT(vkCreateRenderPass(mVkDevice, &info, nullptr, &mVkRenderPass) == VK_SUCCESS, "RenderPass", "Failed to create render pass!");

		CORE_LOG(HE_VERBOSE, "RenderPass", "Render pass created successfully!");

		//Create framebuffer
		VkImageView imageViews[8] = {};
		for (unsigned char i = 0; i < desc.ColorAttachments.size(); i++)
		{
			const VulkanTextureView* pView = (const VulkanTextureView*)desc.ColorAttachments[i].pView;
			imageViews[i] = pView->GetVkTextureView();
		}

		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.width = desc.TargetRenderSize.x;
		framebufferInfo.height = desc.TargetRenderSize.y;
		framebufferInfo.renderPass = mVkRenderPass;
		framebufferInfo.layers = 1;
		framebufferInfo.attachmentCount = attachments.size();
		framebufferInfo.pAttachments = imageViews;
		framebufferInfo.flags = VkFramebufferCreateFlags();
		framebufferInfo.pNext = nullptr;

		DEV_ASSERT(vkCreateFramebuffer(mVkDevice, &framebufferInfo, nullptr, &mVkFramebuffer) == VK_SUCCESS, "VulkanRenderPass", "Failed to create normal framebuffer!");
	}

	void VulkanRenderPass::OnShutdown() noexcept
	{
	}
}