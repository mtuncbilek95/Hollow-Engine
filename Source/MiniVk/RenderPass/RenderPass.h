#pragma once

#include <Core/Core.h>
#include <Renderer/Renderer.h>

#include <vulkan.h>

namespace MiniVk
{
	struct RenderPassDesc
	{

	};

	class RenderPass
	{
	public:
		RenderPass(const RenderPassDesc& desc, Renderer* pRenderer);
		~RenderPass();

		VkRenderPass GetRenderPass() { return mRenderPass; }
		VkFramebuffer GetFramebuffer(uint32 index) { return mFramebuffers[index]; }

	private:
		VkRenderPass mRenderPass;
		VkDevice mDevice;
		Array<VkFramebuffer> mFramebuffers;
	};
}