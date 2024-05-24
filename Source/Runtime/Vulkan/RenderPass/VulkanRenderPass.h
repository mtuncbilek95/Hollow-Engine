#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/RenderPass/RenderPass.h>
#include <Runtime/Vulkan/Device/VulkanDevice.h>

#include <vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanRenderPass : public RenderPass
	{
		VulkanRenderPass(const RenderPassDesc& desc, SharedPtr<VulkanDevice> device);
		virtual ~VulkanRenderPass() override;

		virtual void OnShutdown() override;
	};
}
