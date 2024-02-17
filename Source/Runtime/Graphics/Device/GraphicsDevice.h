#pragma once

#include <Runtime/Core/Core.h>

#include <Runtime/Graphics/Device/GraphicsAPI.h>
#include <Runtime/Graphics/Device/GraphicsDeviceDesc.h>
#include <Runtime/Graphics/Adapter/GraphicsAdapter.h>

#include <Runtime/Graphics/Buffer/GraphicsViewUpdateDesc.h>

namespace Hollow
{
	class GraphicsDeviceObject;
	class Swapchain;
	class Shader;
	class GraphicsView;
	class Texture;
	class TextureView;
	class Sampler;
	class Pipeline;
	class ResourceLayout;
	class CommandView;
	class RenderPass;

	struct SwapchainDesc;
	struct ShaderDesc;
	struct GraphicsViewDesc;
	struct TextureDesc;
	struct TextureViewDesc;
	struct SamplerDesc;
	struct GraphicsPipelineDesc;
	struct ComputePipelineDesc;
	struct ResourceLayoutDesc;
	struct CommandViewDesc;
	struct RenderPassDesc;

	/**
	 * @class GraphicsDevice
	 * 
	 * @brief It is a both abstraction layer and usable interface via 
	 * higher abstractions. Responsible for creating and managing the
	 * objects that are related to the graphics device.
	 * 
	 */
	class RUNTIME_API GraphicsDevice
	{
	public:
		GraphicsDevice(const GraphicsDeviceDesc& desc);
		virtual ~GraphicsDevice() = default;

		SharedPtr<Swapchain>				CreateSwapchain(const SwapchainDesc& desc);
		SharedPtr<Shader>					CreateShader(const ShaderDesc& desc);
		SharedPtr<GraphicsView>				CreateGraphicsView(const GraphicsViewDesc& desc);
		SharedPtr<Texture>					CreateTexture(const TextureDesc& desc);
		SharedPtr<TextureView>				CreateTextureView(const TextureViewDesc& desc);
		SharedPtr<Sampler>					CreateSampler(const SamplerDesc& desc);
		SharedPtr<Pipeline>					CreateGraphicsPipeline(const GraphicsPipelineDesc& desc);
		SharedPtr<Pipeline>					CreateComputePipeline(const ComputePipelineDesc& desc);
		SharedPtr<ResourceLayout>			CreateResourceLayout(const ResourceLayoutDesc& desc);
		SharedPtr<CommandView> 				CreateCommandView(const CommandViewDesc& desc);
		SharedPtr<RenderPass>				CreateRenderPass(const RenderPassDesc& desc);

		// Mostly will be used to submit command views to the GPU
		void SubmitCommandView(const ArrayList<SharedPtr<CommandView>>& commandViews, const byte amount);

		// Mostly will be used to call swapchain via GraphicsManager
		const SharedPtr<Swapchain>& GetSwapchain() const { return mSwapchain; }
		const SharedPtr<GraphicsAdapter>& GetGraphicsAdapter() const { return mGraphicsAdapter; }

		GraphicsAPI GetGraphicsAPI() const { return mGraphicsAPI; }

	protected:
		virtual SharedPtr<Swapchain>		CreateSwapchainCore(const SwapchainDesc& desc) = 0;
		virtual SharedPtr<Shader>			CreateShaderCore(const ShaderDesc& desc) = 0;
		virtual SharedPtr<GraphicsView>		CreateGraphicsViewCore(const GraphicsViewDesc& desc) = 0;
		virtual SharedPtr<Texture>			CreateTextureCore(const TextureDesc& desc) = 0;
		virtual SharedPtr<TextureView>		CreateTextureViewCore(const TextureViewDesc& desc) = 0;
		virtual SharedPtr<Sampler>			CreateSamplerCore(const SamplerDesc& desc) = 0;
		virtual SharedPtr<Pipeline>			CreateGraphicsPipelineCore(const GraphicsPipelineDesc& desc) = 0;
		virtual SharedPtr<Pipeline>			CreateComputePipelineCore(const ComputePipelineDesc& desc) = 0;
		virtual SharedPtr<ResourceLayout>	CreateResourceLayoutCore(const ResourceLayoutDesc& desc) = 0;
		virtual SharedPtr<CommandView>		CreateCommandViewCore(const CommandViewDesc& desc) = 0;
		virtual SharedPtr<RenderPass>		CreateRenderPassCore(const RenderPassDesc& desc) = 0;

		virtual void SubmitCommandViewCore(const ArrayList<SharedPtr<CommandView>>& commandViews, const byte amount) = 0;

	protected:
		ArrayList<SharedPtr<GraphicsDeviceObject>> mDeviceObjects;
		SharedPtr<Swapchain> mSwapchain;
		GraphicsAPI mGraphicsAPI;
		SharedPtr<GraphicsAdapter> mGraphicsAdapter;
	};
}
