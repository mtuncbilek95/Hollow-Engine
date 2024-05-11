#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/RenderPass/RenderPass.h>
#include <Runtime/Vulkan/Device/VulkanDevice.h>

#include <vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanRenderPass final : public RenderPass
	{
	public:
		VulkanRenderPass(const RenderPassDesc& desc, SharedPtr<VulkanDevice> pDevice);
		~VulkanRenderPass() override = default;

		VkRenderPass GetVkRenderPass() const { return mVkRenderPass; }
		VkFramebuffer GetVkFramebuffer() const { return mVkFramebuffer; }

		// Inherited via RenderPass
		void OnShutdown() noexcept override;

	private:
		VkRenderPass mVkRenderPass;
		VkFramebuffer mVkFramebuffer;
		VkDevice mVkDevice;
	};
}
