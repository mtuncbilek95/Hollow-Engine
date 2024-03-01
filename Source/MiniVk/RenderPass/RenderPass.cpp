#include "RenderPass.h"

namespace MiniVk
{
	RenderPass::RenderPass(const RenderPassDesc& desc, Renderer* pRenderer)
	{
		mDevice = pRenderer->GetVkLogicalDevice();

		// Color attachment
		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = pRenderer->GetVkSwapchainImageFormat();
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		// Color attachment reference
		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;

		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorAttachment;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.pDependencies = &dependency;
		renderPassInfo.dependencyCount = 1;

		DEV_ASSERT(vkCreateRenderPass(mDevice, &renderPassInfo, nullptr, &mRenderPass) == VK_SUCCESS, "RenderPass", "Failed to create render pass!");

		CORE_LOG(HE_VERBOSE, "RenderPass", "Render pass created successfully!");

		mFramebuffers.resize(pRenderer->GetVkSwapchainImageViews().size());

		for(uint32 i = 0; i < mFramebuffers.size(); i++)
		{
			VkImageView attachments[] = {
				pRenderer->GetVkSwapchainImageViews()[i]
			};

			VkFramebufferCreateInfo framebufferInfo{};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = mRenderPass;
			framebufferInfo.attachmentCount = 1;
			framebufferInfo.pAttachments = attachments;
			framebufferInfo.width = pRenderer->GetWindowSize().x;
			framebufferInfo.height = pRenderer->GetWindowSize().y;
			framebufferInfo.layers = 1;

			DEV_ASSERT(vkCreateFramebuffer(mDevice, &framebufferInfo, nullptr, &mFramebuffers[i]) == VK_SUCCESS, "RenderPass", "Failed to create framebuffer!");
		}
	}

	RenderPass::~RenderPass()
	{
		for (size_t i = 0; i < mFramebuffers.size(); i++)
		{
			vkDestroyFramebuffer(mDevice, mFramebuffers[i], nullptr);
		}

		vkDestroyRenderPass(mDevice, mRenderPass, nullptr);

	}
}