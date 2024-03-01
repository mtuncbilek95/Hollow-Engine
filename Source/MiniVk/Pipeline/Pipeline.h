#pragma once

#include <Core/Core.h>
#include <Renderer/Renderer.h>

namespace MiniVk
{
	class Shader;
	class RenderPass;
	struct GraphicsPipelineDesc
	{
		Array<Shader*> Shaders;
		RenderPass* pRenderPass;

	};

	class Pipeline
	{
	public:
		Pipeline(const GraphicsPipelineDesc& desc, Renderer* pRenderer);
		~Pipeline();

		VkPipeline GetPipeline() const { return mPipeline; }
		VkPipelineLayout GetPipelineLayout() const { return mPipelineLayout; }
		VkDevice GetDevice() const { return mDevice; }

		const Array<Shader*>& GetShaders() const { return mShaders; }
		RenderPass* GetRenderPass() const { return mRenderPass; }

	private:
		VkPipeline mPipeline;
		VkPipelineLayout mPipelineLayout;
		VkDevice mDevice;

		Array<Shader*> mShaders;
		RenderPass* mRenderPass;
	};
}