#include "VulkanRenderPass.h"

#include <Runtime/Vulkan/Texture/VulkanTextureUtils.h>
#include <Runtime/Vulkan/RenderPass/VulkanRenderPassUtils.h>

#include <Runtime/Vulkan/Texture/VulkanTextureView.h>

namespace Hollow
{
	VulkanRenderPass::VulkanRenderPass(const RenderPassDesc& desc, VulkanDevice* pDevice) : RenderPass(desc, pDevice), mVkDevice(pDevice->GetVkDevice()),
		mVkRenderPass(VK_NULL_HANDLE)
	{
		// Create color attachment
		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = VulkanTextureUtils::GetVkTextureFormat(desc.Format);
		colorAttachment.samples = VulkanTextureUtils::GetVkSampleCount(desc.SampleCount);
		colorAttachment.loadOp = VulkanRenderPassUtils::GetVkAttachmentLoadOp(desc.ColorLoadOperation);
		colorAttachment.storeOp = VulkanRenderPassUtils::GetVkAttachmentStoreOp(desc.ColorStoreOperation);
		colorAttachment.stencilLoadOp = VulkanRenderPassUtils::GetVkAttachmentLoadOp(desc.StencilLoadOperation);
		colorAttachment.stencilStoreOp = VulkanRenderPassUtils::GetVkAttachmentStoreOp(desc.StencilStoreOperation);
		colorAttachment.initialLayout = VulkanTextureUtils::GetVkTextureMemoryLayout(desc.InputLayout);
		colorAttachment.finalLayout = VulkanTextureUtils::GetVkTextureMemoryLayout(desc.OutputLayout);

		// Create color attachment reference
		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		// Create subpass
		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;

		// Create subpass dependency
		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		// Create render pass
		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorAttachment;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		DEV_ASSERT(vkCreateRenderPass(mVkDevice, &renderPassInfo, nullptr, &mVkRenderPass) == VK_SUCCESS, "VulkanRenderPass", "Failed to create render pass!");

		CORE_LOG(HE_VERBOSE, "VulkanRenderPass", "Render pass created successfully!");

		mVkFramebuffers.resize(desc.Views.size());

		for (uint32 i = 0; i < mVkFramebuffers.size(); i++)
		{
			VkImageView attachments[] = {
				static_cast<VulkanTextureView*>(desc.Views[i])->GetVkTextureView()
			};

			VkFramebufferCreateInfo framebufferInfo{};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = mVkRenderPass;
			framebufferInfo.attachmentCount = 1;
			framebufferInfo.pAttachments = attachments;
			framebufferInfo.width = desc.TargetRenderSize.x;
			framebufferInfo.height = desc.TargetRenderSize.y;
			framebufferInfo.layers = 1;

			DEV_ASSERT(vkCreateFramebuffer(mVkDevice, &framebufferInfo, nullptr, &mVkFramebuffers[i]) == VK_SUCCESS, "RenderPass", "Failed to create framebuffer!");
		}
	}

	void VulkanRenderPass::OnShutdown() noexcept
	{
		// Remove from last to first
		for (int i = mVkFramebuffers.size() - 1; i >= 0; i--)
		{
			if(mVkFramebuffers[i] != VK_NULL_HANDLE)
				vkDestroyFramebuffer(mVkDevice, mVkFramebuffers[i], nullptr);
		}

		if(mVkRenderPass != VK_NULL_HANDLE)
			vkDestroyRenderPass(mVkDevice, mVkRenderPass, nullptr);

		mVkFramebuffers.clear();

		mVkRenderPass = VK_NULL_HANDLE;
		mVkDevice = VK_NULL_HANDLE;

		CORE_LOG(HE_VERBOSE, "VulkanRenderPass", "Render pass destroyed successfully!");
	}
}