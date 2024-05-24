#include "VulkanRenderPass.h"

namespace Hollow
{
	VulkanRenderPass::VulkanRenderPass(const RenderPassDesc& desc, SharedPtr<VulkanDevice> device) : RenderPass(desc, device)
	{
	}

	VulkanRenderPass::~VulkanRenderPass()
	{
	}

	void VulkanRenderPass::OnShutdown()
	{
	}
}
