#include "VulkanRenderPass.h"

#include <Runtime/Vulkan/Core/VulkanCoreUtils.h>
#include <Runtime/Vulkan/Texture/VulkanTextureUtils.h>

namespace Hollow
{
	VulkanRenderPass::VulkanRenderPass(const RenderPassDesc& desc, SharedPtr<VulkanDevice> device) : RenderPass(desc, device), mLogicalDevice(device->GetVkDevice())
	{
		// Color Attachments
		ArrayList<VkAttachmentDescription> attachmentDescriptions;
		for (auto& attachment : desc.ColorAttachments)
		{
			VkAttachmentDescription attachmentDescription = {};
			attachmentDescription.format = VulkanTextureUtils::GetVkTextureFormat(attachment.AttachmentFormat);
			attachmentDescription.samples = VulkanTextureUtils::GetVkSampleCount(attachment.SampleCount);
			attachmentDescription.loadOp = VulkanCoreUtils::GetVkAttachmentLoadOperation(attachment.ColorLoadOp);
			attachmentDescription.storeOp = VulkanCoreUtils::GetVkAttachmentStoreOperation(attachment.ColorStoreOp);
			attachmentDescription.stencilLoadOp = VulkanCoreUtils::GetVkAttachmentLoadOperation(attachment.StencilLoadOp);
			attachmentDescription.stencilStoreOp = VulkanCoreUtils::GetVkAttachmentStoreOperation(attachment.StencilStoreOp);
			attachmentDescription.initialLayout = VulkanTextureUtils::GetVkTextureMemoryLayout(attachment.InitialLayout);
			attachmentDescription.finalLayout = VulkanTextureUtils::GetVkTextureMemoryLayout(attachment.FinalLayout);

			attachmentDescriptions.push_back(attachmentDescription);
		}

		// Input Attachments
		ArrayList<VkAttachmentDescription> inputAttachmentDescriptions;
		for (auto& attachment : desc.InputAttachments)
		{
			VkAttachmentDescription attachmentDescription = {};
			attachmentDescription.format = VulkanTextureUtils::GetVkTextureFormat(attachment.AttachmentFormat);
			attachmentDescription.samples = VulkanTextureUtils::GetVkSampleCount(attachment.SampleCount);
			attachmentDescription.loadOp = VulkanCoreUtils::GetVkAttachmentLoadOperation(attachment.ColorLoadOp);
			attachmentDescription.storeOp = VulkanCoreUtils::GetVkAttachmentStoreOperation(attachment.ColorStoreOp);
			attachmentDescription.stencilLoadOp = VulkanCoreUtils::GetVkAttachmentLoadOperation(attachment.StencilLoadOp);
			attachmentDescription.stencilStoreOp = VulkanCoreUtils::GetVkAttachmentStoreOperation(attachment.StencilStoreOp);
			attachmentDescription.initialLayout = VulkanTextureUtils::GetVkTextureMemoryLayout(attachment.InitialLayout);
			attachmentDescription.finalLayout = VulkanTextureUtils::GetVkTextureMemoryLayout(attachment.FinalLayout);

			inputAttachmentDescriptions.push_back(attachmentDescription);
		}

		// Resolve Attachment
		VkAttachmentDescription resolveAttachmentDescription = {};
		if (desc.bHasResolveAttachment)
		{
			resolveAttachmentDescription.format = VulkanTextureUtils::GetVkTextureFormat(desc.ResolveAttachment.AttachmentFormat);
			resolveAttachmentDescription.samples = VulkanTextureUtils::GetVkSampleCount(desc.ResolveAttachment.SampleCount);
			resolveAttachmentDescription.loadOp = VulkanCoreUtils::GetVkAttachmentLoadOperation(desc.ResolveAttachment.ColorLoadOp);
			resolveAttachmentDescription.storeOp = VulkanCoreUtils::GetVkAttachmentStoreOperation(desc.ResolveAttachment.ColorStoreOp);
			resolveAttachmentDescription.stencilLoadOp = VulkanCoreUtils::GetVkAttachmentLoadOperation(desc.ResolveAttachment.StencilLoadOp);
			resolveAttachmentDescription.stencilStoreOp = VulkanCoreUtils::GetVkAttachmentStoreOperation(desc.ResolveAttachment.StencilStoreOp);
			resolveAttachmentDescription.initialLayout = VulkanTextureUtils::GetVkTextureMemoryLayout(desc.ResolveAttachment.InitialLayout);
			resolveAttachmentDescription.finalLayout = VulkanTextureUtils::GetVkTextureMemoryLayout(desc.ResolveAttachment.FinalLayout);
		}

		// Depth Stencil Attachment
		VkAttachmentDescription depthStencilAttachmentDescription = {};
		if (desc.bHasDepthStencilAttachment)
		{
			depthStencilAttachmentDescription.format = VulkanTextureUtils::GetVkTextureFormat(desc.DepthStencilAttachment.AttachmentFormat);
			depthStencilAttachmentDescription.samples = VulkanTextureUtils::GetVkSampleCount(desc.DepthStencilAttachment.SampleCount);
			depthStencilAttachmentDescription.loadOp = VulkanCoreUtils::GetVkAttachmentLoadOperation(desc.DepthStencilAttachment.ColorLoadOp);
			depthStencilAttachmentDescription.storeOp = VulkanCoreUtils::GetVkAttachmentStoreOperation(desc.DepthStencilAttachment.ColorStoreOp);
			depthStencilAttachmentDescription.stencilLoadOp = VulkanCoreUtils::GetVkAttachmentLoadOperation(desc.DepthStencilAttachment.StencilLoadOp);
			depthStencilAttachmentDescription.stencilStoreOp = VulkanCoreUtils::GetVkAttachmentStoreOperation(desc.DepthStencilAttachment.StencilStoreOp);
			depthStencilAttachmentDescription.initialLayout = VulkanTextureUtils::GetVkTextureMemoryLayout(desc.DepthStencilAttachment.InitialLayout);
			depthStencilAttachmentDescription.finalLayout = VulkanTextureUtils::GetVkTextureMemoryLayout(desc.DepthStencilAttachment.FinalLayout);
		}

		// Color Attachment Reference
		ArrayList<VkAttachmentReference> colorAttachmentReference;
		for (uint32_t i = 0; i < desc.ColorAttachments.size(); i++)
		{
			VkAttachmentReference attachmentReference = {};
			attachmentReference.attachment = i;
			attachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

			colorAttachmentReference.push_back(attachmentReference);
		}

		// Input Attachment Reference
		ArrayList<VkAttachmentReference> inputAttachmentReference;
		for (uint32_t i = 0; i < desc.InputAttachments.size(); i++)
		{
			VkAttachmentReference attachmentReference = {};
			attachmentReference.attachment = i;
			attachmentReference.layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

			inputAttachmentReference.push_back(attachmentReference);
		}

		// Depth Stencil Attachment Reference
		VkAttachmentReference depthStencilAttachmentReference = {};
		if (desc.bHasDepthStencilAttachment)
		{
			depthStencilAttachmentReference.attachment = 1;
			depthStencilAttachmentReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		}

		// Resolve Attachment Reference
		VkAttachmentReference resolveAttachmentReference = {};
		if (desc.bHasResolveAttachment)
		{
			resolveAttachmentReference.attachment = 1;
			resolveAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		}

		// Subpass Description
		VkSubpassDescription subpassDescription = {};
		subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpassDescription.colorAttachmentCount = static_cast<uint32_t>(colorAttachmentReference.size());
		subpassDescription.pColorAttachments = colorAttachmentReference.data();
		subpassDescription.pDepthStencilAttachment = desc.bHasDepthStencilAttachment ? &depthStencilAttachmentReference : nullptr;
		subpassDescription.pResolveAttachments = desc.bHasResolveAttachment ? &resolveAttachmentReference : nullptr;
		subpassDescription.inputAttachmentCount = static_cast<uint32_t>(inputAttachmentReference.size());
		subpassDescription.pInputAttachments = inputAttachmentReference.data();
		subpassDescription.flags = VkSubpassDescriptionFlags();

		// Subpass Dependency
		VkSubpassDependency subpassDependency = {};
		subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		subpassDependency.dstSubpass = 0;
		subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		subpassDependency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		// Gather total attachments
		ArrayList< VkAttachmentDescription> totalAttachments = attachmentDescriptions;
		if (desc.bHasDepthStencilAttachment)
			totalAttachments.push_back(depthStencilAttachmentDescription);
		if (desc.bHasResolveAttachment)
			totalAttachments.push_back(resolveAttachmentDescription);
		for (auto& inputAttachment : inputAttachmentDescriptions)
			totalAttachments.push_back(inputAttachment);

		VkRenderPassCreateInfo renderPassCreateInfo = {};
		renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassCreateInfo.attachmentCount = static_cast<uint32_t>(totalAttachments.size());
		renderPassCreateInfo.pAttachments = totalAttachments.data();
		renderPassCreateInfo.subpassCount = 1;
		renderPassCreateInfo.pSubpasses = &subpassDescription;
		renderPassCreateInfo.dependencyCount = 1;
		renderPassCreateInfo.pDependencies = &subpassDependency;

		CORE_ASSERT(vkCreateRenderPass(mLogicalDevice, &renderPassCreateInfo, nullptr, &mRenderPass) == VK_SUCCESS, "VulkanRenderPass", "Failed to create render pass");
	}

	VulkanRenderPass::~VulkanRenderPass()
	{
	}

	void VulkanRenderPass::OnShutdown()
	{
		vkDestroyRenderPass(mLogicalDevice, mRenderPass, nullptr);
	}
}
