#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/RenderPass/RenderPass.h>
#include <Runtime/Vulkan/Device/VulkanDevice.h>

#include <vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanRenderPass : public RenderPass
	{
	public:
		VulkanRenderPass(const RenderPassDesc& desc, VulkanDevice* pDevice);
		~VulkanRenderPass() override = default;

		const VkRenderPass& GetVkRenderPass() const { return mVkRenderPass; }
		const VkFramebuffer& GetVkFramebuffer() const { return mVkFramebuffer; }

		void OnShutdown() noexcept override;

	private:
		VkFramebuffer mVkFramebuffer;
		VkRenderPass mVkRenderPass;
		VkDevice mVkLogicalDevice;
	};
}