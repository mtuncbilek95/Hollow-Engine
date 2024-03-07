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

		VkRenderPass GetVkRenderPass() const { return mVkRenderPass; }
		const Array<VkFramebuffer>& GetVkFramebuffers() const { return mVkFramebuffers; }

		// Inherited via RenderPass
		void OnShutdown() noexcept override;

	private:
		VkRenderPass mVkRenderPass;
		Array<VkFramebuffer> mVkFramebuffers;
		VkDevice mVkDevice;
	};
}